#include "producer.h"

Producer::Producer(const char * filename)
{
    this->filename = filename; //filename
    this->attr.mq_maxmsg = 20; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_prod); //Max size of message
    queue_prod = mq_open(QPROD_NAME , O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if(queue_prod == (mqd_t)-1)
    {
        cerr << "\nP:Not created" << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_prod);
}

Producer::~Producer()
{
    cout << "\nP:~Producer" << endl;
    if (mq_unlink(QPROD_NAME) == -1) {
        cerr << "\nP:mq_unlink" << strerror(errno) << endl;
        exit(1);
    }
}

void Producer::run()
{
    int position=0;
    cout<<"\nP:Producer init"<<endl;
    queue_prod = mq_open(QPROD_NAME, O_RDWR);

    if(queue_prod == (mqd_t)-1)
    {
        cerr << "\nP:In mq_open()" << strerror(errno) << endl;
        exit(1);
    }

    memset(msg_prod.bytes,'\0',sizeof(char)*sizeof(msg_prod.bytes));
    ifstream fp;
#if DECRYPT
    fp.open("cypher.txt", ifstream::in);
#else
    fp.open(filename, ifstream::in);
#endif
    if (!fp){
        std::cout << "\nP:Could not open file" << std::endl;
        exit(0);
    }
    fp.seekg(0, fp.end);
    size_t n = fp.tellg();
    fp.seekg(0, fp.beg);
    for (size_t i = 0; i < n; ){
        if (position >= 64){
            memset(msg_prod.bytes,'\0',sizeof(char)*sizeof(msg_prod.bytes));
            position = 0;
        }
        fp.read(reinterpret_cast<char*>(&msg_prod.bytes), sizeof(msg_prod.bytes)-1);

        cout << "\nP:msg_prod " << msg_prod.bytes << endl;
        int err = mq_send(queue_prod,reinterpret_cast<const char*>(&msg_prod),sizeof(msg_prod),0);
        if (err < 0)
            cerr << "\nP:mq_send " << strerror(errno) << endl;

        position+=fp.gcount();
        i+=fp.gcount();
    }
    mq_close(queue_prod);
    cout<<"\nP:Producer over"<<endl;
}
