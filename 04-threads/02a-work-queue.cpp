/* Real-time Systems
 * Department of Electrical Engineering - DEE
 * Alternative and Renewable Energy Center - CEAR
 * Federal university of Parahyba - UFPB
 * 
 * (C) 2020 Lucas Vinicius Hartmann
 *
 * Description: Thread example, iterating over a work queue.
 * 
 * In this program we calculate pi in the slowest possible way:
 * 1 - Imagine a pixelated circle of known radius R.
 * 2 - Iterate over all pixels on quadrant I, from (0,0) to (R,R).
 * 3 - Count pixels whose center is less-than-R away from (0,0):
 *         square(X+0.5) + square(Y+0.5) < square(R)
 *     Rewriting as ints (0.25+0.25 rounds to 0):
 *         X*X + X * Y*Y + Y < R*R
 * 4 - Counted pixels are approximate area of first quadrant, so:
 *         pi = (counted_pixels * 4) / (r * r)
 * 
 * The total number of pixels to be considered is R*R, which should
 * be pretty large for better approximation. It is possible to split
 * the work across multiple threads, simply having each thread process
 * a subset of the total rows.
 * 
 * Note uint32_t and uint64_t are just Unsigne INTegers of a given bit
 * size. R is 32bit, but R*R may be larger and would overflow. All 
 * variables that could potentially overflow were promoted to 64-bit.
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

// Structure describing parameters to a work packet:
struct argument_t
{
    uint32_t y0, y1; // Start(inclusive) and end(exclusive) rows.
    uint32_t r;      // Radius, to avoid using globals.
};

/* Structure describing the result of each work packet.
 * Provided here for example purposes, but it is generally better
 * to embed output fields directly in the argument structure and
 * avoid dynamic memory allocation (new/delete).
 */
struct result_t
{
    uint64_t a; // Partial area
};

// The worker thread, signature is fixed:
void *thread_function(void *varg)
{
    // Cast the argument from void* to the known type.
    argument_t *arg = (argument_t *)varg;

    // Locals
    uint64_t r = arg->r;
    uint64_t a = 0;

    // Iterate over all rows on the range
    for (uint64_t y = arg->y0; y < arg->y1; y++)
    {
        // Iterate over all pixels on the row
        for (uint64_t x = 0; x < r; x++)
        {
            // If inside R, count
            if (x * x + x + y * y + y < r * r)
                a++;
        }
    }

    // Allocate a result structure. Must use new, not a local, or
    // the data would cease to exist at end of function.
    result_t *res = new result_t;
    res->a = a;
    return res;
}

// Number of parallel threads/cpus to be used
#define NUM_CPU 1

int main()
{
    // Target radius
    uint32_t r = 100000;

    // Number of rows per segment
    // step = rounded_up(r / NUM_CPU)
    uint32_t step = (r + NUM_CPU - 1) / NUM_CPU;

    // Local storage for thread data.
    pthread_t tid[NUM_CPU];
    argument_t arg[NUM_CPU];

    // Configure and start all threads
    for (int i = 0; i < NUM_CPU; i++)
    {
        // Configure
        arg[i].y0 = step * i;
        arg[i].y1 = step * (i + 1);
        arg[i].r = r;

        // Start and error check
        if (pthread_create(&tid[i], 0, thread_function, &arg[i]) < 0)
        {
            cerr << "pthread_create(...) failed!" << endl;
            return -1;
        }
    }

    // All thread are running
    
    // Iteate over all threads collecting results
    uint64_t a = 0;
    for (int i = 0; i < NUM_CPU; i++)
    {
        // Local storage for the results of ONE thread.
        result_t *res;

        /* Wait for thread completion, and capture result. res gets filled
         * with the pointer returned by thread_function. &res is a pointer
         * to the pointer, and has to be cast to type-erased void**.
         * This base interface is quite ugly, but is highly generic. 
         */
        if (pthread_join(tid[i], (void **)&res) < 0) {
            cerr << "pthread_join(...) failed!" << endl;
            return -1;
        }

        // Accumulate result from thread
        a += res->a;

        // And release the allocated memory. Each new NEEDS ONE delete.
        delete res;
    }

    // Print result. Note the dot after 4, making it and all the following 
    // math run as double-precision floating-point (not integers).
    cout << "Pi = " << (a * 4. / r / r) << endl;

    return 0;
}
