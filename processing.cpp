#include "processing.h"

Processing::Processing()
{


}

Processing::~Processing()
{
    cout << "Pro:~Processing" << endl;
//    if (mq_unlink(QPROD_NAME) == -1) {
//        cerr << "\nP:mq_unlink" << strerror(errno) << endl;
//        exit(1);
//    }
//    if (mq_unlink(QCONS_NAME) == -1) {
//        cerr << "\nC:mq_unlink" << strerror(errno) << endl;
//        exit(1);
//    }
}

void Processing::run(){
    cout<<"\nPro:Processing init"<<endl;
    // Best password by consensus.
    uint8_t key[32] = {1, 2, 3, 4, 5, 6};
    // Really does not matter what this is, except that it is only used once.
    uint8_t nonce[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    ChaCha20 chacha(key, nonce);

    // mqd_t queue_prod;
    //mqd_t queue_cons_proc;

    //buff_message_processing temp_msg_pross;
    int msg_proc;
    //struct mq_attr msgq_attr;

    //unsigned int sender;

    //msg_pross.bytes= load_file("rfc7539.txt");

    queue_prod_proc = mq_open(QPROD_NAME, O_RDWR);

    if(queue_prod_proc == (mqd_t)-1)
    {
        cerr << "\nPro:Error opening queue" << strerror(errno) << endl;
        exit(1);
    }
#if DEBUG
    mq_getattr(queue_prod_proc, &attr);
    cout << "Pro:msgq_attr.msgsize " << attr.mq_msgsize << endl;
    cout << "Pro:msgq_attr.maxmsg " << attr.mq_maxmsg << endl;
#endif

    msg_proc = mq_receive(queue_prod_proc, reinterpret_cast<char*>(&msg_pross), sizeof(msg_pross), 0);
    if(msg_proc== -1)
    {
        cerr << "\nPro:Error Receiving" << strerror(errno) << endl;
        exit(1);
    }

    cout << "\nPro:msg_pross " << msg_pross.bytes << endl;

    //temp_msg_pross=msg_pross;
    cout <<"\nPro:Number of bytes: "<< sizeof(msg_pross.bytes)<<endl;
    mq_close(queue_prod_proc);


    //chacha.crypt(&temp_msg_pross.bytes[0], temp_msg_pross.bytes.size());
    chacha.crypt(&msg_pross.bytes[0], sizeof(msg_pross.bytes));

    cout << "\nPro:msg_pross post crypt" << msg_pross.bytes << endl;
    sleep(10);
    queue_cons_proc = mq_open(QCONS_NAME , O_RDWR);

    if(queue_cons_proc == (mqd_t)-1)
    {
        cerr << "\nPro:In mq_open()" << strerror(errno) << endl;
        exit(1);
    }

    mq_send(queue_cons_proc,reinterpret_cast<const char*>(&msg_pross),sizeof(msg_pross),0);
    mq_close(queue_cons_proc);
    cout<<"\nPro:Processing over"<<endl;
    while(1);
}

//Bytes Processing::load_file(const char *path){
//    ifstream fp;
//    fp.open(path, ifstream::in);
//    if (!fp){
//        std::cout << "Could not open file " << path << std::endl;
//        exit(0);
//    }
//    fp.seekg(0, fp.end);
//    size_t n = fp.tellg();
//    fp.seekg(0, fp.beg);
//    Bytes bytes(n);
//    fp.read(reinterpret_cast<char*>(&bytes[0]), n);
//    fp.close();
//    return bytes;
//}

//void Processing::save_file(const char *path, const Bytes &bytes){
//    ofstream fp;
//    fp.open(path, ofstream::out | ofstream::app);
//    fp.write(reinterpret_cast<const char*>(&bytes[0]),bytes.size());
//    fp.close();
//}
