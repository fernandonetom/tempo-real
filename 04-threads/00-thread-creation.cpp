/* Department of Electrical Engineering - DEE
 * Alternative and Renewable Energy Center - CEAR
 * Federal university of Parahyba - UFPB
 * 
 * Real-time Systems
 * 
 * Lucas Vinicius Hartmann
 *
 * Description: Poorly-threaded, zero-player, ping-pong game.
 */

#include <iostream>
#include <unistd.h>
#include <pthread.h> // POSIX threads

using namespace std;

// All threads must abide to this signature:
// Single argument..: void*
// Return value.....: void*
void *pong_thread(void *) {
    for (int i=0; i<15; i++) {
        cout << "      pong" << endl;
        usleep(200000);
    }
    return 0;
}

int main() {
    pthread_t handle;
    if (pthread_create(&handle, 0, pong_thread, 0) < 0) {
        cerr << "pthread_create(...) failed." << endl;
    }
    pthread_detach(handle);

    // Play ping pong
    for (int i=0; i<15; i++) {
        cout << "ping" << endl;
        usleep(200000);
    }

    return 0;
}
