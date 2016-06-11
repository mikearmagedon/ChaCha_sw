#include "producer.h"

Producer::Producer()
{
    this->queue_prod = -1;
    this->filename = NULL;
    this->attr.mq_maxmsg = QUEUE_SIZE; //max # of messages
    this->attr.mq_msgsize = sizeof(msg_prod); //Max size of message
    this->ID = -1;
    memset(&queue_name, 0, sizeof(queue_name));
}

Producer::~Producer()
{
#if DEBUG_P
    cout << "\nP:~Producer" << endl;
#endif
    if (mq_unlink(queue_name) == -1) {
        cerr << "\nP:mq_unlink" << strerror(errno) << endl;
        exit(1);
    }
}

void Producer::set_ID(int id) {
    this->ID = id;
}

void Producer::set_filename(const char * filename) {
    this->filename = filename;
}

char Producer::get_filename() {
    return *(this->filename);
}

void Producer::run()
{
#if DEBUG_P
    cout<<"\nP:Producer init"<<endl;
#endif
    if (ID == -1) {
        cerr << "\nP:Invalid thread ID:" << ID << endl;
        exit(1);
    }

    sprintf(queue_name, "/queue_prod%d", ID);
    int position=0;

    queue_prod = mq_open(queue_name, O_CREAT, S_IRWXU | S_IRWXG, &(this->attr));

    if(queue_prod == (mqd_t)-1)
    {
        cerr << "\nP:Not created" << strerror(errno) << endl;
        exit(1);
    }

    mq_close(queue_prod);

    queue_prod = mq_open(queue_name, O_RDWR);

    if(queue_prod == (mqd_t)-1)
    {
        cerr << "\nP:In mq_open()" << strerror(errno) << endl;
        exit(1);
    }

    memset(msg_prod.bytes,'\0',sizeof(char)*sizeof(msg_prod.bytes));
    ifstream fp;

    fp.open(filename, ifstream::in | ios::binary);

    if (!fp){
        cerr << "\nP:Could not open file" << endl;
        exit(1);
    }
    fp.seekg(0, fp.end);
    size_t n = fp.tellg();
    fp.seekg(0, fp.beg);

    int no_of_blks = ceil(n/64.0);
#if DEBUG_P
    cout << "\nP:no_of_blks " << no_of_blks << endl;
#endif

    for (size_t i = 0; i < n; ){
        if (position >= 64){
            memset(msg_prod.bytes,'\0',sizeof(char)*sizeof(msg_prod.bytes));
            position = 0;
        }
        fp.read(reinterpret_cast<char*>(&msg_prod.bytes), sizeof(msg_prod.bytes)-1);
#if DEBUG_P
        cout << "\nP:msg_prod " << msg_prod.bytes << endl;
#endif
        msg_prod.numb_bytes_read = fp.gcount(); // set number of bytes read
        msg_prod.no_of_blocks = no_of_blks--; // set block number
#if DEBUG_P
        cout << "\nP:msg_prod.no_of_blocks " << msg_prod.no_of_blocks << endl;
        cout << "\nP:sizeof(msg_prod.bytes) " << sizeof(msg_prod.bytes) << endl;
        cout << "\nP:msg_prod.numb_bytes_read " << msg_prod.numb_bytes_read << endl;
#endif
        int err = mq_send(queue_prod, reinterpret_cast<const char*>(&msg_prod), sizeof(msg_prod), 0);
        if (err < 0) {
            cerr << "\nP:mq_send " << strerror(errno) << endl;
            exit(1);
        }
        // Signal Processing thread
        union sigval value;
        memset(&value, 0, sizeof(value));
        value.sival_int = ID;
        if (sigqueue(getpid(), RTSIGNAL_P, value) != 0) {
            cerr << "\nP:sigqueue " << strerror(errno) << endl;
            exit(1);
        }
        position+=fp.gcount();
        i+=fp.gcount();
    }
    mq_close(queue_prod);
#if DEBUG_P
    cout<<"\nP:Producer over"<<endl;
#endif
    pthread_exit(NULL);
}
