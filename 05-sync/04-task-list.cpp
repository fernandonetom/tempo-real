/*
 * Departamento de Engenharia Elétrica - UFPB
 * Sistemas em Tempo Real
 * Lucas Vinicius Hartmann
 *
 * Descrição:
 * Semaphore signaling
 */
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <vector>
#include <string>

using namespace std;

/* Defines runner_t as the type of a thread function, so we may cast the
 * function during pthread_create, instead of the argument on the
 * function. It changes nothing, just another option.
 */
typedef void* (*worker_function_t)(void*);

struct job_queue_t {
    sem_t sem_in;
    sem_t sem_out;
	pthread_mutex_t lock;
    vector<string> jobs;
    int ptr_in;
    int ptr_out;
};

void push_job(job_queue_t &jq, string s) {
    // Changing the queue creates races, so lock it.
    pthread_mutex_lock(&jq.lock);

    // Put the job on the queue
    jq.jobs[jq.ptr_in] = s;
    jq.ptr_in = (jq.ptr_in + 1) % jq.jobs.size();

    // No more risk of races now.
    pthread_mutex_unlock(&jq.lock);

    // Let other threads know of the new job
    sem_post(&jq.sem_in);
}

string pop_job(job_queue_t &jq) {
    // Wait for a job
    sem_wait(&jq.sem_in);

    // The mutex protects que queue and indexes.
    pthread_mutex_lock(&jq.lock);

    // Take a job from the queue
    string s = jq.jobs[jq.ptr_out];
    jq.ptr_out = (jq.ptr_out + 1) % jq.jobs.size();

    // No more risk of races now.
    pthread_mutex_unlock(&jq.lock);

    return s;
}

struct worker_argument_t {
    pthread_t tid;
    int id, count;
    job_queue_t *jq;
};

// Defines a thread function, but with appropriate arg pointer type.
void *worker(worker_argument_t *arg) {
    while(true) {
        // Take a job from the queue
        string s = pop_job(*arg->jq);
        
        // Do the job
        cout << "Worker [" << arg->id << "] working on " << s << "..." << endl;
        sleep(1); // pretend it taka a second.
        cout << "Worker [" << arg->id << "] completed " << s << "." << endl;
        arg->count++;

        // Signal job completed
        sem_post(&arg->jq->sem_out);
    }
	return 0;
}

// MAIN
#define NUM_THREADS 6
int main() {
    job_queue_t jq;
	pthread_mutex_init(&jq.lock, 0);
    sem_init(&jq.sem_in, 0, 0);
    sem_init(&jq.sem_out, 0, 0);
    jq.ptr_in = 0;
    jq.ptr_out = 0;
    jq.jobs.resize(50);

	// Spawn worker threads
	worker_argument_t workers[NUM_THREADS];
	for (int i=0; i<NUM_THREADS; ++i) {
        workers[i].id = i;
        workers[i].jq = &jq;
        workers[i].count = 0;
		pthread_create(&workers[i].tid, 0, (worker_function_t)worker, &workers[i]);
	}

    // Push jobs on the queue
    push_job(jq, "John");
    push_job(jq, "Jane");
    push_job(jq, "Josh");
    push_job(jq, "Jack");
    push_job(jq, "Jonathan");
    push_job(jq, "Jasper");
    push_job(jq, "Jill");
    push_job(jq, "Jaspion");

    // Wait for all jobs to complete
    cout << "Waiting for jobs to complete..." << endl;
    for (int i=0; i<8; i++)
        sem_wait(&jq.sem_out);
	
	// The workers have no way to end, so cancel before joining
	for (int i=0; i<NUM_THREADS; ++i) {
		pthread_cancel(workers[i].tid);
        pthread_join(workers[i].tid, 0);
        cout << "Worker " << workers[i].id << " executed " << workers[i].count << " jobs." << endl;
    }

	pthread_mutex_destroy(&jq.lock);
    sem_destroy(&jq.sem_in);
    sem_destroy(&jq.sem_out);
	
	return 0;
}
