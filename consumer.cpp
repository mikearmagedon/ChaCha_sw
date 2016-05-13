#include "consumer.h"

Consumer::Consumer()
{
    this->attr.mq_maxmsg = 20; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_cons); //Max size of message
    this->ID = -1;
}

Consumer::~Consumer()
{
    cout << "\nC:~Consumer" << endl;
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
        cout << "\nP:Invalid thread ID:" << ID << endl;
        exit(1);
    }

    sprintf(queue_name, "/queue_cons%d", ID);

    queue_cons = mq_open(queue_name , O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Not created" << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_cons);

    queue_cons = mq_open(queue_name, O_RDWR);

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Error opening queue" << strerror(errno) << endl;
        exit(1);
    }
    mq_getattr(queue_cons, &attr);

    ofstream fp;
    char name[strlen(filename)+6];
    strcpy(name, filename);
    strcat(name, ".rslt");
    cout << "\nC:name " << name << endl;
    cout << "\nC:ifstream " << ifstream(name) << endl;

    if (ifstream(name)!=0) {
        cout << "\nC:Removing file ..." <<endl;
        remove(name);
    }

    cout<<"\nC:Waiting to receive"<<endl;
    do{
        msg_con_int = mq_receive(queue_cons, reinterpret_cast<char*>(&msg_cons),sizeof(msg_cons),&sender);
        if(msg_con_int == -1)
        {
            cerr << "\nC:Error Receiving Consumer " << strerror(errno) << endl;
            exit(1);
        }
        cout << "\nC:blk_#:" << msg_cons.no_of_blocks << endl;
        cout << "\nC:msg_cons " << msg_cons.bytes << endl;
        cout << "\nC:Consumer Number of bytes: "<< sizeof(msg_cons.bytes)/sizeof(uint8_t)<<endl;


        cout << "\nC:saving " << msg_cons.numb_bytes_read << " bytes" << endl;
        fp.open(name, ofstream::out | ios::binary | ofstream::app);
        cout << "\nC:Writing to file ..." << endl;
        fp.write(reinterpret_cast<const char*>(&msg_cons.bytes[0]), msg_cons.numb_bytes_read);
        fp.close();
        //#if DECRYPT
        //    save_file("plain.txt",msg_cons);
        //#else
        //save_file(filename, msg_cons);
        //#endif
    } while (--(msg_cons.no_of_blocks) > 0);
    mq_close(queue_cons);
    cout << "\nC:Consumer Over" << endl;
    pthread_exit(NULL);
}
