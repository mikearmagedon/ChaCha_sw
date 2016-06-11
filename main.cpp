#include "producer.h"
#include "consumer.h"
#include "processing.h"

int main(int argc, char *argv[])
{
#if DEBUG
    cout << "PID: " << getpid() << endl;
    cout << "argc " << argc << endl;
    for (int i=0; i<argc; i++) {
        cout << "argv[" << i << "] " << argv[i] << endl;
    }
#endif
    //Block signals RTSIGNAL_P and SIGINT for all threads
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, RTSIGNAL_P);
    sigaddset(&set, SIGINT);
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
        cerr << "\nM:pthread_sigmask " << strerror(errno) << endl;
        exit(1);
    }

    // Initialize and start set of producer and consumer threads
    Producer prod[NO_OF_THREADS];
    Consumer cons[NO_OF_THREADS];
    char file_name[20];
    for (int i = 0; i < NO_OF_THREADS; i++) {
        sprintf(file_name, "file%d", i);
        prod[i].set_filename(file_name);
        prod[i].set_ID(i);
        if (prod[i].Start(PRIORITY))
        {
            cerr << "\nM:Couldn't create" << endl;
            exit (0);
        }
        cons[i].set_filename(file_name);
        cons[i].set_ID(i);
        if (cons[i].Start(PRIORITY))
        {
            cerr << "\nM:Couldn't create" << endl;
            exit (0);
        }
    }
    //Initialize processing thread
    // Best password by consensus.
    uint8_t key[32] = {KEY};
    // Really does not matter what this is, except that it is only used once.
    uint8_t nonce[12] = {NONCE};
    Processing proc(key, nonce);

    //Start running processing thread
    if(proc.Start(1))
    {
        cerr << "\nM:Couldn't create" << endl;
        exit (0);
    }

    //Join set of producer and consumer threads
#if DEBUG
    cout << "\nM:Waiting for threads to finish..." << endl;
#endif
    for (int i = 0; i < NO_OF_THREADS; i++) {
        pthread_join(prod[i].pthread, 0);
#if DEBUG
        cout << "\nM:Join prod[" << i << "]" << endl;
#endif
        pthread_join(cons[i].pthread, 0);
#if DEBUG
        cout << "\nM:Join cons[" << i << "]" << endl;
#endif
    }

    //Join processing thread
#if DEBUG
    cout << "\nM:Waiting for processing thread to finish..." << endl;
#endif
    pthread_join(proc.pthread, 0);

    //Clean-up
    for (int i = 0; i < NO_OF_THREADS; i++) {
        prod[i].~Producer();
        cons[i].~Consumer();
    }
#if DEBUG
    cout << "\nM:Exiting..." << endl;
#endif
    exit(0);
}

