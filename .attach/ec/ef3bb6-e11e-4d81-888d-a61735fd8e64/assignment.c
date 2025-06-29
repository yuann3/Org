/* assignment.c — Viridien Software Developer Technical Test
 *
 * Author: Yiyuan Li
 * Date: 1 July
 */

#include <stddef.h>  /* size_t */
#include <stdio.h>   /* printf */
#include <stdlib.h>  /* EXIT_SUCCESS*/

/*
 * I. SEARCH
 * ---------
 * Given an integer array a of length n and an integer x,
 * return the index of x within a.
 *
 * Complexity :   O(n) time   |   O(1) space
 *
 * Agrs:
 *   a    : input array
 *   n    : size of input array
 *   x    : number that we are searching for
 *
 * Return:
 *   i      x's index
 *   -1     any invalid argument
 */

int search(const int *a, size_t n, int x) {
    if (a == NULL || n == 0)
        return -1;

    for (size_t i = 0; i < n; ++i) {
        if (a[i] == x)
            return (int)i;             /* found */
    }

    return -1;                         /* not found */
}


/*
 * III. CONVOLUTION
 * ---------
 * Linear convolution of two equal-length float arrays a and b.
 *
 * Complexity : O(n^2) time | O(1) space
 *
 *              i use double-sum (brute force approach), in this function
 *              which is more cache friendly and beats FFT for small/medium n
 *
 *              but if n is around 1-2k, FFT → point-wise multiply → iFFT
 *              drops the work to O(n log n)
 *
 * Args:
 *   a, b  : input arrays, both len n
 *   n     : length of a and b
 *   c     : output array, len: 2*n-1
 *
 * Return:
 *   0       success
 *   -1      invalid argument
 */
int convolution(const float *a, const float *b, size_t n, float *c) {
    if (!a || !b || !c || n == 0)
        return -1;

    /* we zero fill the ourput buffer first*/
    const size_t m = 2 * n - 1;
    for (size_t i = 0; i < m; ++i)
        c[i] = 0.0f;

    for (size_t i = 0; i < n; ++i) {
        const float ai = a[i];
        float *row = c + i;       /* c[i+j] */
        for (size_t j = 0; j < n; ++j)
            row[j] += (float)((double)ai * (double)b[j]);
    }
    return 0;
}


/* ----------------
 * TEST DRIVER
 * ----------------
 * Build with:
 *   $ cc -std=c11 -Wall -Wextra -pedantic assignment.c -o assignment
 */
static void selftest_search(void)
{
    puts("== I.SEARCH() tests ==");
    {
        int a[] = {1, 2, 3, 4, 5};
        const size_t n = sizeof a / sizeof *a;
        printf("- expect index 2:  %d\n", search(a, n, 3));
        printf("- expect -1:       %d\n", search(a, n, 42));
    }
    {
        /* edge-cases */
        printf("- NULL array:      %d\n", search(NULL, 5, 1));
        printf("- n == 0 length:   %d\n", search(NULL, 0, 1));
    }
}

static void selftest_convolution(void)
{
    puts("\n== III.CONVOLUTION() tests ==");
    {
        float a[] = {1.0f, 2.0f, 3.0f};
        float b[] = {4.0f, 5.0f, 6.0f};
        const size_t n = sizeof a / sizeof *a;
        float c[2 * sizeof a / sizeof *a - 1];
        convolution(a, b, n, c);
        puts("- expect  4 13 28 27 18:");
        puts("- result:");
        for (size_t i = 0; i < 2 * n - 1; ++i)
            printf("%.0f ", c[i]);
        putchar('\n');
    }
    {   /* edge-case: n == 1 */
        float a[] = {3.14f};
        float c[1];
        const int rc = convolution(a, a, 1, c);
        printf("- n==1 round-trip, rc=%d, c[0]=%.2f (expect 9.86)\n",
               rc, c[0]);
    }
}

int main(void)
{
    selftest_search();
    selftest_convolution();
    return EXIT_SUCCESS;
}
