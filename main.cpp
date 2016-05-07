#include <QCoreApplication>
#include <iostream>

#include "producer.h"
#include "consumer.h"
#include "processing.h"

#ifndef    QPROD_NAME
#define    QPROD_NAME    "/queue_prod"
#endif

#ifndef    QCONS_NAME
#define    QCONS_NAME    "/queue_cons"
#endif

int main(int argc, char *argv[])
{
#if DEBUG
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

    Producer prod[atoi(argv[1])];
    Consumer cons[atoi(argv[1])];
    for (int i = 0; i < atoi(argv[1]); i++) {
        prod[i].set_filename(argv[i+2]);
        cons[i].set_filename(argv[i+2]);
    }

    Processing proc;

    if(cons[0].Start(1))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }
    if(prod[0].Start(1))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }
    if(proc.Start(1, true))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }

    cout << "\nM:Waiting for threads to finish..." << endl;
    pthread_join(prod[0].pthread, 0);
    pthread_join(cons[0].pthread, 0);

    cout << "\nM:Exiting..." << endl;
    exit(0);
}

