/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include <assert.h> /* for assert */
#include <float.h> /* for DBL_MAX */
#include <string.h> /* for memset, memcpy */
#include "macros.h"
#include "top_n_outlier_pruning_block.h"

#ifdef DEBUG
    #define LOGGING
    
    #ifndef LOG_FILE
        #define LOG_FILE "top_n_outlier_pruning_block.log"
    #endif /* #ifndef LOG_FILE */
#endif /* #ifdef DEBUG */

#ifdef LOGGING
    #include <stdio.h> /* for fprintf */
#endif /* #ifdef LOGGING */
/******************************************************************************/

/******************************************************************************/
/* Check compatibility of defined macros.                                     */
/******************************************************************************/
#if (defined(UNSORTED_INSERT) && defined(SORTED_INSERT)) || (!defined(UNSORTED_INSERT) && !defined(SORTED_INSERT))
    #error "Exactly one of UNSORTED_INSERT and SORTED_INSERT should be defined."
#endif /* #if defined(UNSORTED_INSERT) && defined(SORTED_INSERT) */
/******************************************************************************/

/* Forward declarations */
static inline double_t distance_squared(
    const size_t vector_dims,
    const double_t (* const vector1)[vector_dims],
    const double_t (* const vector2)[vector_dims]
    );
#ifdef SORTED_INSERT
static inline double_t sorted_insert(
    const size_t k,
    index_t (* const outliers)[k],
    double_t (* const outlier_scores)[k],    
    uint_t * const found,
    const index_t new_outlier, 
    const double_t new_outlier_index
    );
#endif /* #ifdef SORTED_INSERT */
#ifdef UNSORTED_INSERT
static inline double_t unsorted_insert(
    const size_t k,
    index_t (* const outliers)[k],
    double_t (* const outlier_scores)[k],
    uint_t * const found,
    const index_t new_outlier, 
    const double_t new_outlier_score
    );
#endif /* #ifdef UNSORTED_INSERT */
static inline void best_outliers(
    const size_t N,
    size_t * const outliers_size,
    index_t (*const outliers)[N],
    double_t (*const outlier_scores)[N],
    const size_t block_size,
    index_t (*const current_block)[block_size],
    double_t (*const scores)[block_size]
    );
static inline void sort_vectors_descending(
    const size_t block_size,
    index_t (* const current_block)[block_size],
    double_t (* const scores)[block_size]
    );
static inline void merge(
    const size_t N,
    const size_t global_outliers_size,
    index_t (* const global_outliers)[N],
    double_t (* const global_outlier_scores)[N],
    const size_t block_size,
    index_t (* const local_outliers)[block_size],
    double_t (* const local_outlier_scores)[block_size],
    size_t * const new_outliers_size,
    index_t (* const new_outliers)[N],
    double_t (* const new_outlier_scores)[N]
    );

static inline double_t distance_squared(const size_t vector_dims,
                                       const double_t (* const vector1)[vector_dims],
                                       const double_t (* const vector2)[vector_dims]) {
#ifndef __AUTOESL__
    assert(vector1 != NULL);
    assert(vector2 != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(vector_dims > 0);
    
    double_t sum_of_squares = 0;
    
    uint_t dim;
    for (dim = 0; dim < vector_dims; dim++) {
        const double_t val         = (*vector1)[dim] - (*vector2)[dim];
        const double_t val_squared = val * val;
        sum_of_squares            += val_squared;
    }
    
    return sum_of_squares;
}

#ifdef SORTED_INSERT
static inline double_t sorted_insert(const size_t k,
                                     index_t (* const outliers)[k],
                                     double_t (* const outlier_scores)[k],
                                     uint_t * const found,
                                     const index_t new_outlier, 
                                     const double_t new_outlier_score) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(outliers != NULL);
    assert(outlier_scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(k > 0);
#ifndef __AUTOESL__
    assert(found != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(*found <= k);
    assert(new_outlier >= start_index);
    
    int_t    insert_index  = -1; /* the index at which the new outlier will be inserted */
    double_t removed_value = -1; /* the value that was removed from the outlier_scores array */
    
    /*
     * Shuffle array elements from front to back. Elements greater than the new
     * value will be right-shifted by one index in the array.
     *
     * Note that uninitialised values in the array will appear on the left. That
     * is, if the array is incomplete (has a size n < N) then the data in the
     * array is stored in the rightmost n indexes.
     */
    
    if (*found < k) {
        /* Special handling required if the array is incomplete. */
        
        uint_t i;
        for (i = k - *found - 1; i < k; i++) {
            if (new_outlier_score > (*outlier_scores)[i] || i == (k - *found - 1)) {
                /* Shuffle values down the array. */
                if (i > 0) {
                    (*outliers      )[i-1] = (*outliers      )[i];
                    (*outlier_scores)[i-1] = (*outlier_scores)[i];
                }
                insert_index  = i;
                removed_value = 0;
            } else {
                /* We have found the insertion point. */
                break;
            }
        }
    } else {
        int_t i;
        for (i = k-1; i >= 0; i--) {
            if (new_outlier_score < (*outlier_scores)[i]) {
                if ((unsigned) i == k-1)
                    /*
                     * The removed value is the value of the last element in the
                     * array.
                     */
                    removed_value = (*outlier_scores)[i];

                /* Shuffle values down the array. */
                if (i > 0) {
                    (*outliers      )[i] = (*outliers      )[i-1];
                    (*outlier_scores)[i] = (*outlier_scores)[i-1];
                }
                insert_index = i;
            } else {
                /* We have found the insertion point. */
                break;
            }
        }
    }

    /*
     * Insert the new pair and increment the current_size of the array (if
     * necessary).
     */
    if (insert_index >= 0) {
        (*outliers      )[insert_index] = new_outlier;
        (*outlier_scores)[insert_index] = new_outlier_score;
        
        if (*found < k)
            (*found)++;
    }
    
    return removed_value;
}
#endif /* #ifdef SORTED_INSERT */

#ifdef UNSORTED_INSERT
static inline double_t unsorted_insert(const size_t k,
                                       index_t (* const outliers)[k],
                                       double_t (* const outlier_scores)[k],
                                       uint_t * const found,
                                       const index_t new_outlier, 
                                       const double_t new_outlier_score) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(outliers != NULL);
    assert(outlier_scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(k > 0);
#ifndef __AUTOESL__
    assert(found != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(*found <= k);
    assert(new_outlier >= start_index);
    
    int_t    insert_index  = -1;  /* the index at which the new outlier will be inserted */
    double_t removed_value = -1; /* the value that was removed from the outlier_scores vector */
    
    if (*found < k) {
        insert_index  = *found + 1;
        removed_value = 0;
    } else {
        int_t    max_index = -1;
        double_t max_value = DBL_MAX;
    
        int_t i;
        for (i = k-1; i >= 0; i--) {
            if (max_index <= 0 || (*outlier_scores)[i] > max_value) {
                max_index = i;
                max_value = (*outlier_scores)[i];
            }
        }
        
        if (new_outlier_score < max_value) {
            insert_index  = max_index;
            removed_value = max_value;
        }
    }

    /*
     * Insert the new pair and increment the current_size of the array (if
     * necessary).
     */
    if (insert_index >= 0) {
        (*outliers      )[insert_index] = new_outlier;
        (*outlier_scores)[insert_index] = new_outlier_score;
        
        if (*found < k)
           (*found)++;
    }
    
    return removed_value;
}
#endif /* #ifdef UNSORTED_INSERT */

static inline void best_outliers(const size_t N, size_t * outliers_size,
                                 index_t (*const outliers)[N],
                                 double_t (*const outlier_scores)[N],
                                 const size_t block_size,
                                 index_t (* const current_block)[block_size],
                                 double_t (* const scores)[block_size]
                                 ) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(outliers != NULL);
    assert(outlier_scores != NULL);
    assert(outliers_size != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(*outliers_size <= N);
    assert(N > 0);
#ifndef __AUTOESL__
    assert(current_block != NULL);
    assert(scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(block_size > 0);
    
    /* Sort the (current_block, scores) vectors in descending order of value. */
    sort_vectors_descending(block_size, current_block, scores);
    
    /* Create two temporary vectors for the output of the "merge" function. */
    index_t  new_outliers      [N];
    double_t new_outlier_scores[N];
    size_t   new_outliers_size = 0;
    
    memset(new_outliers,       null_index, N * sizeof(index_t));
    memset(new_outlier_scores,          0, N * sizeof(double_t));
    
    /* Merge the two vectors. */
    merge(N, *outliers_size, outliers, outlier_scores, block_size, current_block, scores, &new_outliers_size, &new_outliers, &new_outlier_scores);
    
    /* Copy values from temporary vectors to real vectors. */
    memcpy(*outliers,       new_outliers,       N * sizeof(index_t));
    memcpy(*outlier_scores, new_outlier_scores, N * sizeof(double_t));
    *outliers_size = new_outliers_size;
}

static inline void sort_vectors_descending(const size_t block_size,
                                           index_t (* const current_block)[block_size],
                                           double_t (* const scores)[block_size]) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(current_block != NULL);
    assert(scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(block_size > 0);
    
    uint_t i;
    for (i = 0; i < block_size; i++) {
    	int_t j;
    	index_t  ind = (*current_block)[i];
        double_t val = (*scores       )[i];
        for (j = i-1; j >= 0; j--) {
            if ((*scores)[j] >= val)
                break;
            (*current_block)[j+1] = (*current_block)[j];
            (*scores       )[j+1] = (*scores       )[j];
        }
        (*current_block)[j+1] = ind;
        (*scores       )[j+1] = val;
    }
}

static inline void merge(const size_t N,
                         const size_t global_outliers_size, index_t (* const global_outliers)[N], double_t (* const global_outlier_scores)[N],
                         const size_t block_size, index_t (* const local_outliers)[block_size], double_t (* const local_outlier_scores)[block_size],
                         size_t * const new_outliers_size, index_t (* const new_outliers)[N], double_t (* const new_outlier_scores)[N]) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(global_outliers != NULL);
    assert(global_outlier_scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(global_outliers_size <= N);
    assert(N > 0);
#ifndef __AUTOESL__
    assert(local_outliers != NULL);
    assert(local_outlier_scores != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(block_size > 0);
#ifndef __AUTOESL__
    assert(new_outliers != NULL);
    assert(new_outlier_scores != NULL);
    assert(new_outliers_size != NULL);
#endif /* #ifndef __AUTOESL__ */
    
    *new_outliers_size  = 0;
    uint_t iter         = 0; /* iterator through output array */
    uint_t global       = 0; /* iterator through global array */
    uint_t local        = 0; /* iterator through local array */
    while (iter < N && (global < global_outliers_size || local < block_size)) {
        if (global >= global_outliers_size && local < block_size) {
            /* There are no remaining elements in the global arrays. */
            (*new_outliers      )[iter] = (*local_outliers      )[local];
            (*new_outlier_scores)[iter] = (*local_outlier_scores)[local];
            local ++;
            global++;
        } else if (global < global_outliers_size && local >= block_size) {
            /* There are no remaining elements in the local arrays. */
            (*new_outliers      )[iter] = (*global_outliers      )[global];
            (*new_outlier_scores)[iter] = (*global_outlier_scores)[global];
            local ++;
            global++;
        } else if (global >= global_outliers_size && local >= block_size) {
            /*
             * There are no remaining elements in either the global or local 
             * arrays.
             */
            break;
        } else if (global_outlier_scores[global] >= local_outlier_scores[local]) {
            (*new_outliers      )[iter] = (*global_outliers      )[global];
            (*new_outlier_scores)[iter] = (*global_outlier_scores)[global];
            global++;
        } else if (global_outlier_scores[global] <= local_outlier_scores[local]) {
            (*new_outliers      )[iter] = (*local_outliers      )[local];
            (*new_outlier_scores)[iter] = (*local_outlier_scores)[local];
            local++;
        }
        
        iter++;
        (*new_outliers_size)++;
    }
}

void top_n_outlier_pruning_block(const size_t num_vectors, const size_t vector_dims,
                                 const double_t (* const data)[num_vectors][vector_dims],
                                 const size_t k, const size_t N, const size_t default_block_size,
                                 index_t (*outliers)[N], double_t (*outlier_scores)[N]) {
    /* Error checking. */
#ifndef __AUTOESL__
    assert(data != NULL);
#endif /* #ifndef __AUTOESL__ */
    assert(vector_dims > 0);
    assert(k > 0);
    assert(N > 0);
    assert(default_block_size > 0);
#ifndef __AUTOESL__
    assert(outliers != NULL);
    assert(outlier_scores != NULL);
#endif /* #ifndef __AUTOESL__ */

    /* Set output to zero. */
#ifndef __AUTOESL__
    memset(*outliers,       null_index, N * sizeof(index_t));
    memset(*outlier_scores,          0, N * sizeof(double_t));
#else
    uint_t i;
    for (i = 0; i < N; i++) {
        (*outliers      )[i] = null_index;
        (*outlier_scores)[i] = 0;
    }
#endif /* #ifndef __AUTOESL__ */
    
    double_t cutoff = 0;            /* vectors with a score less than the cutoff will be removed from the block */
    size_t   outliers_found = 0;    /* the number of initialised elements in the outliers array */
    index_t  block_begin;           /* the index of the first vector in the block currently being processed */
    size_t   block_size;            /* block_size may be smaller than devfault_block_size if "num_vectors mod default_block_size != 0" */
    
#ifdef LOGGING
    FILE * log_file;
    log_file = fopen(LOG_FILE, "a+");
#ifndef __AUTOESL__
    assert(log_file != NULL);
#endif /* #ifndef __AUTOESL__ */
#endif /* #ifdef LOGGING */
    
    for (block_begin = 0; block_begin < num_vectors; block_begin += block_size) { /* while there are still blocks to process */
        block_size = MIN(block_begin + default_block_size, num_vectors) - block_begin; /* the number of vectors in the current block */
        assert(block_size <= default_block_size);
        
        index_t current_block[block_size];          /* the indexes of the vectors in the current block */
        index_t neighbours[block_size][k];          /* the "k" nearest neighbours for each vector in the current block */
        double_t neighbours_dist[block_size][k];    /* the distance of the "k" nearest neighbours for each vector in the current block */
        double_t score[block_size];                 /* the average distance to the "k" neighbours */
        uint_t found[block_size];                   /* how many nearest neighbours we have found, for each vector in the block */
        
        /* Reset array contents */
        uint_t i;
        for (i = 0; i < block_size; i++)
            current_block[i] = (index_t)((block_begin + i) + start_index);
        memset(&neighbours,      null_index, block_size * k * sizeof(index_t));
        memset(&neighbours_dist,          0, block_size * k * sizeof(double));
        memset(&score,                    0, block_size * sizeof(double));
        memset(&found,                    0, block_size * sizeof(uint_t));
        
#ifdef LOGGING
        fprintf(log_file, "Processing block %u..%u\n", (unsigned int) block_begin, (unsigned int) (block_begin + block_size + start_index -1));
#endif /* #ifdef LOGGING */
        
        index_t vector1;
        for (vector1 = start_index; vector1 < num_vectors + start_index; vector1++) {
            uint_t block_index;
            for (block_index = 0; block_index < block_size; block_index++) {
                const index_t vector2 = current_block[block_index];
                
                if (vector1 != vector2 && vector2 >= start_index) {
                    /*
                     * Calculate the square of the distance between the two
                     * vectors (indexed by "vector1" and "vector2")
                     */
                    const double_t dist_squared = distance_squared(vector_dims, &((*data)[vector1-start_index]), &((*data)[vector2-start_index]));
#ifdef LOGGING
                    fprintf(log_file, "Distance(%u,%u) = %f\n", (unsigned int) vector2, (unsigned int) vector1, dist_squared);
#endif /* #ifdef LOGGING */

                    /*
                     * Insert the new (index, distance) pair into the neighbours
                     * array for the current vector.
                     */
#ifdef SORTED_INSERT
                    const double_t removed_distance = sorted_insert  (k, &(neighbours[block_index]), &(neighbours_dist[block_index]), &(found[block_index]), vector1, dist_squared);
#endif /* #ifdef SORTED_INSERT */
#ifdef UNSORTED_INSERT
                    const double_t removed_distance = unsorted_insert(k, &(neighbours[block_index]), &(neighbours_dist[block_index]), &(found[block_index]), vector1, dist_squared);
#endif /* #ifdef UNSORTED_INSERT */
#ifdef LOGGING
                    uint_t log_i;
                    if (block_size > 0) {
                        fprintf(log_file, "Neighbours(%u):\n", (unsigned int) vector2);
                        for (log_i = 0; log_i < block_size; log_i++)
                            fprintf(log_file, "\t%u @ %f\n", (unsigned int) neighbours[block_index][log_i], neighbours_dist[block_index][log_i]);
                    }
#endif /* #ifdef LOGGING */
                    
                    /*
                     * Update the score (if the neighbours array was changed).
                     */
                    if (removed_distance >= 0)
                        score[block_index] = (double_t) ((score[block_index] * k - removed_distance + dist_squared) / k);
                    
                    /*
                     * If the score for this vector is less than the cutoff,
                     * then prune this vector from the block.
                     */
                    if (found[block_index] >= k && score[block_index] < cutoff) {
                        current_block[block_index] = null_index;
                        score        [block_index] = 0;
                    }
                   
#ifdef LOGGING
                    if (block_size > 0) {
                        fprintf(log_file, "Block:\n");
                        for (log_i = 1; log_i < block_size; log_i++)
                            fprintf(log_file, "\t%u @ %f\n", (unsigned int) current_block[log_i], score[log_i]);
                    }
#endif /* #ifdef LOGGING */
                }
            }
        }
        
        /* Keep track of the top "N" outliers. */
        best_outliers(N, &outliers_found, outliers, outlier_scores, block_size, &current_block, &score);
#ifdef LOGGING
        fprintf(log_file, "count(outliers) = %u\n", (unsigned int) outliers_found);
        uint_t log_i;
        if (N > 0) {
            fprintf(log_file, "Outliers:\n");
            for (log_i = 0; log_i < N; log_i++)
                fprintf(log_file, "\t%u @ %f\n", (unsigned int) (*outliers)[log_i], (*outlier_scores)[log_i]);
        }
#endif /* #ifdef LOGGING */
        
        /*
         * Set "cutoff" to the score of the weakest outlier. There is no need to
         * store an outlier in future iterations if its score is better than the
         * cutoff.
         */
        cutoff = (*outlier_scores)[N-1];
#ifdef LOGGING
        fprintf(log_file, "cutoff = %f\n", cutoff);
#endif /* #ifdef LOGGING */
    }
    
#ifdef LOGGING
    fclose(log_file);
#endif /* #ifdef LOGGING */
}
