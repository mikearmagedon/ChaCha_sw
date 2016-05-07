#include "consumer.h"

Consumer::Consumer()
{
    this->attr.mq_maxmsg = 20; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_cons); //Max size of message

    queue_cons = mq_open(QCONS_NAME , O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Not created" << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_cons);
}

Consumer::~Consumer()
{
    cout << "\nC:~Consumer" << endl;
    if (mq_unlink(QCONS_NAME) == -1) {
        cerr << "\nC:mq_unlink" << strerror(errno) << endl;
        exit(1);
    }
}

void Consumer::set_filename(char * filename) {
    this->filename = filename;
}

void Consumer::run()
{
    queue_cons = mq_open(QCONS_NAME, O_RDWR);

    if(queue_cons == (mqd_t)-1)
    {
        cerr << "\nC:Error opening queue" << strerror(errno) << endl;
        exit(1);
    }
    mq_getattr(queue_cons, &attr);
    cout<<"C:Waiting to receive"<<endl;
    msg_con_int = mq_receive(queue_cons, reinterpret_cast<char*>(&msg_cons),sizeof(msg_cons),&sender);//Must send the received value for verification
    if(msg_con_int == -1)
    {
        cerr << "\nC:Error Receiving Consumer" << strerror(errno) << endl;
        exit(1);
    }

    cout << "\nC:msg_cons " << msg_cons.bytes << endl;
    cout << "\nC:Consumer Number of bytes: "<< sizeof(msg_cons.bytes)/sizeof(uint8_t)<<endl;
    mq_close(queue_cons);
//#if DECRYPT
//    save_file("plain.txt",msg_cons);
//#else
    save_file(filename, msg_cons);
//#endif
}

void Consumer::save_file(char *path, const buff_message_cons &src_msg){
    ofstream fp;
    char name[strlen(path)+6];
    strcpy(name, path);
    strcat(name, ".rslt");
    cout << "\nC:saving " << src_msg.numb_bytes_read << " bytes" << endl;
    cout << "\nC:name " << name << endl;
    cout << "\nC:ifstream " << ifstream(name) << endl;
//
    if (ifstream(name)!=0) {
        cout << "Removing file ..." <<endl;
        remove(name);
    }
    fp.open(name, ofstream::out | ofstream::app);
    cout << "Writing to file ..." << endl;
    fp.write(reinterpret_cast<const char*>(&src_msg.bytes[0]), src_msg.numb_bytes_read);
    fp.close();
    cout<<"\nC:End it"<<endl;
}
