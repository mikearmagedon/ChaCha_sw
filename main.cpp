#include <QCoreApplication>
#include "producer.h"
#include "consumer.h"
#include "processing.h"

int main(int argc, char *argv[])
{
    cout << "PID: " << getpid() << endl;
#if DEBUG
    cout << "PID: " << getpid() << endl;
    cout << "argc " << argc << endl;
    for (int i=0; i<argc; i++) {
        cout << "argv[" << i << "] " << argv[i] << endl;
    }
#endif
    if (argc == 1) {
        cout << "Too few arguments" << endl;
        cout << "Usage: Case_Study #_of_threads filenames" << endl;
        exit(0);
    }
    else if (atoi(argv[1]) != argc-2) { //Check input arguments
        cout << "The # of files provided doesn't match with the # of threads" << endl;
        exit(0);
    }
    else if (atoi(argv[1]) <= 0 || atoi(argv[1]) > 10) {
        cout << "The # of threads provided must be between 1 and 10" << endl;
        exit(0);
    }
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
    Producer prod[atoi(argv[1])];
    Consumer cons[atoi(argv[1])];
    for (int i = 0; i < atoi(argv[1]); i++) {
        prod[i].set_filename(argv[i+2]);
        prod[i].set_ID(i);
        if (prod[i].Start(1))
        {
            cerr << "\nM:Couldn't create" << endl;
            exit (0);
        }
        cons[i].set_filename(argv[i+2]);
        cons[i].set_ID(i);
        if (cons[i].Start(1))
        {
            cerr << "\nM:Couldn't create" << endl;
            exit (0);
        }
    }

    //Initialize processing thread
    // Best password by consensus.
    uint8_t key[32] = {1, 2, 3, 4, 5, 6};
    // Really does not matter what this is, except that it is only used once.
    uint8_t nonce[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Processing proc(key, nonce);

    //Start running processing thread
    if(proc.Start(1))
    {
        cerr << "\nM:Couldn't create" << endl;
        exit (0);
    }

    //Join set of producer and consumer threads
    cout << "\nM:Waiting for threads to finish..." << endl;
    for (int i = 0; i < atoi(argv[1]); i++) {
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
    cout << "\nM:Waiting for processing thread to finish..." << endl;
    pthread_join(proc.pthread, 0);

    //Clean-up
    for (int i = 0; i < atoi(argv[1]); i++) {
        prod[i].~Producer();
        cons[i].~Consumer();
    }
    cout << "\nM:Exiting..." << endl;
    exit(0);
}

