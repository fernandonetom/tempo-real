/* Real-time Systems
 * Department of Electrical Engineering - DEE
 * Alternative and Renewable Energy Center - CEAR
 * Federal university of Parahyba - UFPB
 * 
 * (C) 2020 Lucas Vinicius Hartmann
 *
 * Description: Same as previous, but avoiding dynamic memory
 * allocation with new/delete for results.
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

// Structure containg all data to a work packet:
struct argument_t
{
    uint32_t y0, y1; // Start(inclusive) and end(exclusive) rows.
    uint32_t r;      // Radius, to avoid using globals.
    uint64_t a;      // Partial area (OUTPUT FIELD)
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

    // Just store back on the argument structure
    arg->a = a;

    return 0;
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
        // Wait for thread completion.
        if (pthread_join(tid[i], 0) < 0) {
            cerr << "pthread_join(...) failed!" << endl;
            return -1;
        }

        // Accumulate result, by reading from the argument structs
        a += arg[i].a;
    }

    // All done, print result.
    cout << "Pi = " << (a * 4. / r / r) << endl;

    return 0;
}
