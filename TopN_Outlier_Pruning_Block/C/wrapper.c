/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "test.h" /* for test */
#include <stdio.h> /* for fprintf, stderr */
/*----------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    /* Check for proper number of arguments. */
    if (argc < 1)
        fprintf(stderr, "No input file specified.\n");
    
    const char * const filename = argv[1];
    return test(filename);
}
