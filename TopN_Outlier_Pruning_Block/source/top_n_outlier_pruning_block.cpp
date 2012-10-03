/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "checks.h" /* check for invalid preprocessor macro combinations */
#include "arch.h" /* set architecture specific macros */

#include "top_n_outlier_pruning_block.h" /* main include file */
#include "utility.h" /* for ASSERT, boolean, double_t, double_in_t, double_io_t, double_out_t, false, INCREMENT_UINT_T, index_t, int_t, MIN, NULL_INDEX, START_INDEX, size_t, size_in_t, true, uint_t */

#include <float.h> /* for DBL_MIN */

#ifdef __AUTOESL__
    #include "ap_interfaces.h" /* for AP_INTERFACE */
#endif /* #ifdef __AUTOESL__ */
/*----------------------------------------------------------------------------*/

#ifndef HARDCODED_NUM_VECTORS
size_t num_vectors_value = 0;
inline void set_num_vectors(const size_in_t num_vectors) {
    num_vectors_value = num_vectors;
}
#endif /* #ifndef HARDCODED_NUM_VECTORS */

#ifndef HARDCODED_VECTOR_DIMS
size_t vector_dims_value = 0;
inline void set_vector_dims(const size_in_t vector_dims) {
    vector_dims_value = vector_dims;
}
#endif /* #ifndef HARDCODED_VECTOR_DIMS */

#ifndef HARDCODED_K
size_t k_value = 0;

inline void set_k(const size_in_t k) {
    k_value = k;
}
#endif /* #ifndef HARDCODED_K */

#ifndef HARDCODED_N
size_t N_value = 0;

inline void set_N(const size_in_t N) {
    N_value = N;
}
#endif /* #ifndef HARDCODED_N */

#ifdef BLOCKING
#ifndef HARDCODED_BLOCK_SIZE
size_t block_size_value = 0;

inline void set_block_size(const size_in_t block_size) {
    block_size_value = block_size;
}
#endif /* #ifndef HARDCODED_BLOCK_SIZE */
#endif /* #ifdef BLOCKING */

INLINE void distance_squared(const double_in_t vector1[vector_dims_value],
                             const double_in_t vector2[vector_dims_value],
                             double_out_t * const sum) {
#ifdef __AUTOESL__
    #pragma AP INTERFACE ap_ctrl_hs port=return
#endif /* #ifdef __AUTOESL__ */

#ifdef __AUTOESL__
    /* Define AutoESL native interface behavior for val1 and val2. */
    //AP_INTERFACE(vector1,none);
    //AP_INTERFACE(vector2,none);
    //AP_INTERFACE(sum,none);

    /* Map val1 and val2 from a native AutoESL interface to AXI4-lite. */
    //AP_BUS_AXI4_LITE(val1,BUS_A);
    //AP_BUS_AXI4_LITE(val2,BUS_B);

    /* Map the control of the function to AXI4-lite. */
    //AP_CONTROL_BUS_AXI(CONTROL_BUS);

    /* Create an AXI4-stream interface for arrays A and B. */
    //AP_BUS_AXI_STREAMD(A,INPUT_STREAM);
    //AP_BUS_AXI_STREAMD(B,OUTPUT_STREAM);
#endif /* #ifdef __AUTOESL__ */

    ASSERT(vector_dims_value > 0);

#ifndef __AUTOESL__
    #define SUM_SPLIT (1)
#else
    #define SUM_SPLIT (8)
#endif /* #ifndef __AUTOESL__ */
    
    double_t sum_of_squares__split[SUM_SPLIT] = { 0 };
    
    uint_t dim;
    dimension_loop: for (dim = 0; dim < vector_dims_value; dim++) {
#ifdef __AUTOESL__
    #pragma AP PIPELINE II=1
#endif /* #ifdef __AUTOESL__ */
//#ifndef __AUTOESL__
#if 1
        const double_t vector1_data            = vector1[dim];
        const double_t vector2_data            = vector2[dim];
#else
        const double_t vector1_data            = vector1[dim].data;
        const double_t vector2_data            = vector2[dim].data;
#endif /* #ifndef __AUTOESL__ */
        const double_t diff                    = vector1_data - vector2_data;
        const double_t diff_squared            = diff * diff;
        sum_of_squares__split[dim % SUM_SPLIT] += diff_squared;
    }

    double_t sum_of_squares = 0;
    uint_t i;
    sum_loop: for (i = 0; i < SUM_SPLIT; i++) {
#ifdef __AUTOESL__
    #pragma AP UNROLL
#endif /* #ifdef __AUTOESL__ */
        sum_of_squares += sum_of_squares__split[i];
    }
    
    *sum = sum_of_squares;
}

INLINE double_out_t add_neighbour(index_io_t neighbours[k_value],
                                  double_io_t neighbours_dist[k_value],
                                  uint_t * const found,
                                  const index_in_t new_neighbour,
                                  const double_in_t new_neighbour_dist) {
    /* Error checking. */
    ASSERT(k_value > 0);
    ASSERT(found != NULL);
    ASSERT(*found <= k_value);
    ASSERT(new_neighbour >= START_INDEX);
    
    int_t    insert_index  = -1; /* the index at which the new outlier will be inserted */
    double_t removed_value = -1; /* the value that was removed from the neighbours_dist array */
    
#if defined(SORTED_INSERT)
    /*
     * Shuffle array elements from front to back. Elements greater than the new
     * value will be right-shifted by one index in the array.
     *
     * Note that uninitialised values in the array will appear on the left. That
     * is, if the array is incomplete (has a size n < N) then the data in the
     * array is stored in the rightmost n indexes.
     */
    
    if (*found < k_value) {
        /* Special handling required if the array is incomplete. */
        
        uint_t i;
        for (i = k_value - *found - 1; i < k_value; i++) {
            if (new_neighbour_dist > neighbours_dist[i] || i == (k_value - *found - 1)) {
                /* Shuffle values down the array. */
                if (i > 0) {
                    neighbours     [i-1] = neighbours     [i];
                    neighbours_dist[i-1] = neighbours_dist[i];
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
        for (i = k_value - 1; i >= 0; i--) {
            if (new_neighbour_dist < neighbours_dist[i]) {
                if ((unsigned) i == k_value - 1)
                    /*
                     * The removed value is the value of the last element in the
                     * array.
                     */
                    removed_value = neighbours_dist[i];
                /* Shuffle values down the array. */
                
                if (i > 0) {
                    neighbours     [i] = neighbours     [i-1];
                    neighbours_dist[i] = neighbours_dist[i-1];
                }
                insert_index = i;
            } else {
                /* We have found the insertion point. */
                break;
            }
        }
    }
#elif defined(UNSORTED_INSERT)
    if (*found < k_value) {
        insert_index  = *found;
        removed_value = 0;
    } else {
        int_t    max_index = -1;
        double_t max_value = DBL_MIN;
        
        int_t i;
        for (i = k_value - 1; i >= 0; i--) {
            if (max_index < 0 || neighbours_dist[i] > max_value) {
                max_index = i;
                max_value = neighbours_dist[i];
            }
        }
        
        if (new_neighbour_dist < max_value) {
            insert_index  = max_index;
            removed_value = max_value;
        }
    }
#endif /* #if defined(SORTED_INSERT) */
    
    /*
     * Insert the new pair and increment the current_size of the array (if
     * necessary).
     */
    if (insert_index >= 0) {
        neighbours     [insert_index] = new_neighbour;
        neighbours_dist[insert_index] = new_neighbour_dist;
        
        if (*found < k_value)
            (*found)++;
    }
    
    return removed_value;
}

INLINE void best_outliers(size_io_t * const outliers_size,
                          index_io_t outliers[N_value],
                          double_io_t outlier_scores[N_value],
#if defined(BLOCKING)
                          const size_in_t block_size,
                          const index_in_t current_block[block_size_value],
                          const double_in_t scores[block_size_value]
#elif defined(NO_BLOCKING)
                          const index_in_t vector,
                          const double_in_t score
#endif /* #if defined(BLOCKING) */
                                 ) {
    /* Error checking. */
    ASSERT(outliers_size != NULL);
    ASSERT(*outliers_size <= N_value);
    ASSERT(N_value > 0);
#ifdef BLOCKING
    ASSERT(block_size > 0);
    ASSERT(block_size <= block_size_value);
#endif /* #ifdef BLOCKING */
    
#ifdef BLOCKING
    /* 
     * Sort the (current_block, scores) vectors in descending order of value.
     *
     * Make a copy first, so that we don't need to modify the input block and 
     * score arrays.
     */
    index_t  block_copy [block_size];
    double_t scores_copy[block_size];
    
    MEMCPY_1D(block_copy,  current_block, block_size, sizeof(index_t));
    MEMCPY_1D(scores_copy, scores,        block_size, sizeof(double_t));
    
    sort_block_scores_descending(block_size, block_copy, scores_copy);
#endif /* #ifdef BLOCKING */
    
    /* Create two temporary vectors for the output of the "merge" function. */
    index_t  new_outliers      [N_value];
    double_t new_outlier_scores[N_value];
    size_t   new_outliers_size = 0;
    
    MEMSET_1D(new_outliers,       NULL_INDEX, N_value, sizeof(index_t));
    MEMSET_1D(new_outlier_scores,          0, N_value, sizeof(double_t));
    
    /* Merge the two vectors. */
#if defined(BLOCKING)
    merge(*outliers_size, outliers, outlier_scores, block_size, block_copy, scores_copy, &new_outliers_size, new_outliers, new_outlier_scores);
#elif defined(NO_BLOCKING)
    merge(*outliers_size, outliers, outlier_scores,             vector,     score,       &new_outliers_size, new_outliers, new_outlier_scores);
#endif /* #if defined(BLOCKING) */
    
    /* Copy values from temporary vectors to real vectors. */
    MEMCPY_1D(outliers,       new_outliers,       N_value, sizeof(index_t));
    MEMCPY_1D(outlier_scores, new_outlier_scores, N_value, sizeof(double_t));
    *outliers_size = new_outliers_size;
}

#ifdef BLOCKING
INLINE void sort_block_scores_descending(const size_in_t block_size,
                                         index_io_t indexes[block_size_value],
                                         double_io_t values[block_size_value]) {
    /* Error checking. */
    ASSERT(block_size > 0);
    ASSERT(block_size <= block_size_value);
    
    uint_t i;
    for (i = 0; i < block_size; i++) {
        int_t j;
        index_t  index = indexes[i];
        double_t value = values [i];
        
        for (j = i-1; j >= 0; j--) {
            if (values[j] >= value)
                break;
            indexes[j+1] = indexes[j];
            values [j+1] = values [j];
        }
        
        indexes[j+1] = index;
        values [j+1] = value;
    }
}
#endif /* #ifdef BLOCKING */

INLINE void merge(const size_in_t global_outliers_size, const index_in_t global_outliers[N_value],         const double_in_t global_outlier_scores[N_value],
#if defined(BLOCKING)
                  const size_in_t block_size,           const index_in_t local_outliers[block_size_value], const double_in_t local_outlier_scores[block_size_value],
#elif defined(NO_BLOCKING)
                                                        const index_in_t local_outlier,                    const double_in_t local_outlier_score,
#endif /* #if defined(BLOCKING) */
                  size_out_t * const new_outliers_size, index_out_t new_outliers[N_value],                 double_out_t new_outlier_scores[N_value]) {
    /* Error checking. */
    ASSERT(global_outliers_size <= N_value);
    ASSERT(N_value > 0);
#ifdef BLOCKING
    ASSERT(block_size > 0);
    ASSERT(block_size <= block_size_value);
#endif /* #ifdef BLOCKING */
    ASSERT(new_outliers_size != NULL);
    
    *new_outliers_size  = 0;
    uint_t iter         = 0; /* iterator through output array */
    uint_t global       = 0; /* iterator through global array */
#if defined(BLOCKING)
    uint_t local        = 0; /* iterator through local array */
#elif defined(NO_BLOCKING)
    boolean local       = false;
#endif /* #if defined(BLOCKING) */
    
#if defined(BLOCKING)
    while (iter < N_value && (global < global_outliers_size || local < block_size)) {
        if (global >= global_outliers_size && local < block_size) {
#elif defined(NO_BLOCKING)
    while (iter < N_value && (global < global_outliers_size || !local)) {
        if (global >= global_outliers_size && !local) {
#endif /* #if defined(BLOCKING) */
            /* There are no remaining elements in the global arrays. */
#if defined(BLOCKING)
            new_outliers      [iter] = local_outliers      [local];
            new_outlier_scores[iter] = local_outlier_scores[local];
            local++;
#elif defined(NO_BLOCKING)
            new_outliers      [iter] = local_outlier;
            new_outlier_scores[iter] = local_outlier_score;
            local = true;
#endif /* #if defined(BLOCKING) */
            global++;
#if defined(BLOCKING)
        } else if (global < global_outliers_size && local >= block_size) {
#elif defined(NO_BLOCKING)
        } else if (global < global_outliers_size && local) {
#endif /* #if defined(BLOCKING) */
            /* There are no remaining elements in the local arrays. */
            new_outliers      [iter] = global_outliers      [global];
            new_outlier_scores[iter] = global_outlier_scores[global];
#if defined(BLOCKING)
            local++;
#elif defined(NO_BLOCKING)
            local = true;
#endif /* #if defined(BLOCKING) */
            global++;
#if defined(BLOCKING)
        } else if (global >= global_outliers_size && local >= block_size) {
#elif defined(NO_BLOCKING)
        } else if (global >= global_outliers_size && local) {
#endif /* #if defined(BLOCKING) */
            /*
             * There are no remaining elements in either the global or local 
             * arrays.
             */
            break;
#if defined(BLOCKING)
        } else if (global_outlier_scores[global] >= local_outlier_scores[local]) {
#elif defined(NO_BLOCKING)
        } else if (global_outlier_scores[global] >= local_outlier_score) {
#endif /* #if defined(BLOCKING) */
            new_outliers      [iter] = global_outliers      [global];
            new_outlier_scores[iter] = global_outlier_scores[global];
            global++;
#if defined(BLOCKING)
        } else if (global_outlier_scores[global] <= local_outlier_scores[local]) {
            new_outliers      [iter] = local_outliers      [local];
            new_outlier_scores[iter] = local_outlier_scores[local];
            local++;
#elif defined(NO_BLOCKING)
        } else if (global_outlier_scores[global] <= local_outlier_score) {
            new_outliers      [iter] = local_outlier;
            new_outlier_scores[iter] = local_outlier_score;
            local = true;
#endif /* #if defined(BLOCKING) */
        }
        
        iter++;
        (*new_outliers_size)++;
    }
}

uint_out_t top_n_outlier_pruning_block(const double_in_t data[MAX_NUM_VECTORS(num_vectors_value)][vector_dims_value],
                                       index_out_t outliers[N_value], double_out_t outlier_scores[N_value]) {
    /* Error checking. */
    ASSERT(num_vectors_value > 0);
    ASSERT(vector_dims_value > 0);
    ASSERT(k_value > 0);
    ASSERT(N_value > 0);
#ifdef BLOCKING
    ASSERT(block_size_value > 0);
#endif /* #ifdef BLOCKING */

    /* Number of pruned vectors. */
    uint_t num_pruned = 0;
    
    /* Set output to zero. */
    MEMSET_1D(outliers,       NULL_INDEX, N_value, sizeof(index_t));
    MEMSET_1D(outlier_scores,          0, N_value, sizeof(double_t));
    
    double_t cutoff = 0;            /* vectors with a score less than the cutoff will be removed from the block */
    size_t   outliers_found = 0;    /* the number of initialised elements in the outliers array */
    
#if defined(BLOCKING)
    index_t  block_begin;           /* the index of the first vector in the block currently being processed */
    size_t   block_size;            /* block_size may be smaller than default_block_size if "num_vectors_value mod default_block_size != 0" */
    
    block_loop: for (block_begin = 0; block_begin < num_vectors_value; block_begin += block_size) { /* while there are still blocks to process */
        block_size = MIN(block_begin + block_size_value, num_vectors_value) - block_begin; /* the number of vectors in the current block */
        ASSERT(block_size <= block_size_value);
        
        index_t  current_block[block_size_value];               /* the indexes of the vectors in the current block */
        index_t  neighbours[block_size_value][k_value];         /* the "k" nearest neighbours for each vector in the current block */
        double_t neighbours_dist[block_size_value][k_value];    /* the distance of the "k" nearest neighbours for each vector in the current block */
        double_t score[block_size_value];                       /* the average distance to the "k" neighbours */
        uint_t   found[block_size_value];                       /* how many nearest neighbours we have found, for each vector in the block */
        
        /* Reset array contents */
        do {
            uint_t i;
            for (i = 0; i < block_size; i++)
                current_block[i] = (index_t) ((block_begin + i) + START_INDEX);
        } while (0);
        MEMSET_2D(neighbours,      NULL_INDEX, block_size, k_value, sizeof(index_t));
        MEMSET_2D(neighbours_dist,          0, block_size, k_value, sizeof(double_t));
        MEMSET_1D(score,                    0, block_size,          sizeof(double_t));
        MEMSET_1D(found,                    0, block_size,          sizeof(uint_t));
        
        index_t vector1;
        outer_loop: for (vector1 = START_INDEX; vector1 < num_vectors_value + START_INDEX; vector1++) {
            uint_t block_index;
            inner_loop: for (block_index = 0; block_index < block_size; block_index++) {
                const index_t vector2 = current_block[block_index];
                if (vector1 != vector2 && vector2 >= START_INDEX) {
                    /*
                     * Calculate the square of the distance between the two
                     * vectors (indexed by "vector1" and "vector2")
                     */
                    double_t dist_squared = 0;
                    double_t vector1_in[num_vectors_value];
                    double_t vector2_in[num_vectors_value];

                    uint_t i;
                    for (i = 0; i < num_vectors_value; i++) {
                        vector1_in[i] = data[vector1 - START_INDEX][i];
                        vector2_in[i] = data[vector2 - START_INDEX][i];
                    }
                    distance_squared(vector1_in, vector2_in, &dist_squared);
                    
                    /*
                     * Insert the new (index, distance) pair into the neighbours
                     * array for the current vector.
                     */
                    const double_t removed_distance = add_neighbour(neighbours[block_index], neighbours_dist[block_index], &(found[block_index]), vector1, dist_squared);
                    
                    /*
                     * Update the score (if the neighbours array was changed).
                     */
                    if (removed_distance >= 0)
                        score[block_index] = (double_t) ((score[block_index] * k_value - removed_distance + dist_squared) / k_value);
                    
                    /*
                     * If the score for this vector is less than the cutoff,
                     * then prune this vector from the block.
                     */
                    if (found[block_index] >= k_value && score[block_index] < cutoff) {
                        current_block[block_index] = NULL_INDEX;
                        score        [block_index] = 0;
                        INCREMENT_UINT_T(num_pruned);
                    }
                }
            }
        }
        
        /* Keep track of the top "N" outliers. */
        best_outliers(&outliers_found, outliers, outlier_scores, block_size, current_block, score);
        
        /*
         * Set "cutoff" to the score of the weakest outlier. There is no need to
         * store an outlier in future iterations if its score is better than the
         * cutoff.
         */
        cutoff = outlier_scores[N_value - 1];
    }
#elif defined(NO_BLOCKING)
    index_t vector1;
    outer_loop: for (vector1 = START_INDEX; vector1 < num_vectors_value + START_INDEX; vector1++) {
        index_t  neighbours[k_value];       /* the "k" nearest neighbours for the current vector */
        double_t neighbours_dist[k_value];  /* the distance of the "k" nearest neighbours for the current vector */
        double_t score = 0;                 /* the average distance to the "k" neighbours */
        uint_t   found = 0;                 /* how many nearest neighbours we have found */
        boolean  removed = false;           /* true if vector1 has been pruned */
        
        MEMSET_1D(neighbours,      NULL_INDEX, k_value, sizeof(index_t));
        MEMSET_1D(neighbours_dist,          0, k_value, sizeof(double_t));
        
        index_t vector2;
        inner_loop: for (vector2 = START_INDEX; vector2 < num_vectors_value + START_INDEX && !removed; vector2++) {
            //if (vector1 != vector2) {
            if (1) {
                /*
                 * Calculate the square of the distance between the two
                 * vectors (indexed by "vector1" and "vector2")
                 */
                double_t dist_squared = 0;
                double_t vector1_in[num_vectors_value];
                double_t vector2_in[num_vectors_value];

                uint_t i;
                for (i = 0; i < num_vectors_value; i++) {
                    vector1_in[i] = data[vector1 - START_INDEX][i];
                    vector2_in[i] = data[vector2 - START_INDEX][i];
                }
                distance_squared(vector1_in, vector2_in, &dist_squared);
                
                /*
                 * Insert the new (index, distance) pair into the neighbours
                 * array for the current vector.
                 */
                const double_t removed_distance = add_neighbour(neighbours, neighbours_dist, &found, vector1, dist_squared);
                
                /* Update the score (if the neighbours array was changed). */
                if (removed_distance >= 0)
                    score = (double_t) ((score * k_value - removed_distance + dist_squared) / k_value);
                
                /*
                 * If the score for this vector is less than the cutoff,
                 * then prune this vector from the block.
                 */
                if (found >= k_value && score < cutoff) {
                    removed = true;
                    INCREMENT_UINT_T(num_pruned);
                    break;
                }
            }
        }
        
        if (!removed) {
            /* Keep track of the top "N" outliers. */
            best_outliers(&outliers_found, outliers, outlier_scores, vector1, score);
            
            /*
             * Set "cutoff" to the score of the weakest outlier. There is no need to
             * store an outlier in future iterations if its score is better than the
             * cutoff.
             */
            cutoff = outlier_scores[N_value - 1];
        }
    }
#endif /* #if defined(BLOCKING) */

    /* Reset algorithm parameters. */
#ifndef HARDCODED_NUM_VECTORS
    set_num_vectors(0);
#endif /* #ifndef HARDCODED_NUM_VECTORS */
#ifndef HARDCODED_VECTOR_DIMS
    set_vector_dims(0);
#endif /* #ifndef HARDCODED_VECTOR_DIMS */
#ifndef HARDCODED_K
    set_k(0);
#endif /* #ifndef HARDCODED_K */
#ifndef HARDCODED_N
    set_N(0);
#endif /* #ifndef HARDCODED_N */
#if defined(BLOCKING) && !defined(HARDCODED_BLOCK_SIZE)
    set_block_size(0);
#endif /* #if defined(BLOCKING) && !defined(HARDCODED_BLOCK_SIZE) */
    
    return num_pruned;
}