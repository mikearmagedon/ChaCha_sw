#include "consumer.h"

Consumer::Consumer()
{
    this->msg_con_int = -1;
    this->queue_cons = -1;
    this->filename = NULL;
    this->attr.mq_maxmsg = QUEUE_SIZE; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_cons); //Max size of message
    this->ID = -1;
}

Consumer::~Consumer()
{
#if DEBUG_C
    cout << "\nC:~Consumer" << endl;
#endif
    if (mq_unlink(queue_name) == -1) {
        cerr << "\nC:mq_unlink" << strerror(errno) << endl;
        exit(1);
    }
}

void Consumer::set_ID(int id) {
    this->ID = id;
}

void Consumer::set_filename(char * filename) {
    this->filename = filename;
}

void Consumer::run()
{
    if (ID == -1) {
        cerr << "\nP:Invalid thread ID:" << ID << endl;
        exit(1);
    }

    sprintf(queue_name, "/queue_cons%d", ID);

    queue_cons = mq_open(queue_name , O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Not created " << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_cons);

    queue_cons = mq_open(queue_name, O_RDWR);

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Error opening queue " << strerror(errno) << endl;
        exit(1);
    }
    mq_getattr(queue_cons, &attr);

    ofstream fp;
    char name[strlen(filename)+6];
    strcpy(name, filename);
    strcat(name, ".rslt");
#if DEBUG_C
    cout << "\nC:name " << name << endl;
    cout << "\nC:ifstream " << ifstream(name) << endl;
#endif
    if (ifstream(name)!=0) {
#if DEBUG_C
    	cout << "\nC:Removing file ..." <<endl;
#endif
    	remove(name);
    }
    fp.open(name, ofstream::out | ios::binary | ofstream::app);

    do {
#if DEBUG_C
        cout<<"\nC:Waiting to receive"<<endl;
#endif
        msg_con_int = mq_receive(queue_cons, reinterpret_cast<char*>(&msg_cons),sizeof(msg_cons),0);
        if(msg_con_int == -1)
        {
            cerr << "\nC:Error Receiving Consumer " << strerror(errno) << endl;
            exit(1);
        }
#if DEBUG_C
        cout << "\nC:blk_#:" << msg_cons.no_of_blocks << endl;
        cout << "\nC:msg_cons " << msg_cons.bytes << endl;
        cout << "\nC:Consumer Number of bytes: "<< sizeof(msg_cons.bytes)/sizeof(uint8_t)<<endl;
        cout << "\nC:saving " << msg_cons.numb_bytes_read << " bytes" << endl;
        cout << "\nC:Writing to file ..." << endl;
#endif
        fp.write(reinterpret_cast<const char*>(&msg_cons.bytes[0]), msg_cons.numb_bytes_read);
    } while (--(msg_cons.no_of_blocks) > 0);
    fp.close();
    mq_close(queue_cons);
#if DEBUG_C
    cout << "\nC:Consumer Over" << endl;
#endif
    pthread_exit(NULL);
}
