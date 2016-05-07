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
    Producer prod("sample_32.txt");
    Consumer cons("cypher.txt");
    Processing proc;

    if(cons.Start(1))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }
    if(prod.Start(1))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }
    if(proc.Start(1, true))
    {
        cerr << "\nM:Couldn't' create" << endl;
        exit (0);
    }



    //while(1);
    cout << "\nM:Waiting for threads to finish..." << endl;
    //exit(0);
    pthread_join(prod.pthread, 0);
    pthread_join(cons.pthread, 0);

    cout << "\nM:Exiting..." << endl;
    exit(0);
}

