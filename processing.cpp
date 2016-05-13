#include "processing.h"

char Processing::src_queue_name[20] = {0};
char Processing::dst_queue_name[20] = {0};

Processing::Processing()
{

}

void Processing::run(){
    cout<<"\nPro:Processing init"<<endl;

    // Best password by consensus.
    uint8_t key[32] = {1, 2, 3, 4, 5, 6};
    // Really does not matter what this is, except that it is only used once.
    uint8_t nonce[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    ChaCha20 chacha(key, nonce);
    int msg_proc;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    while(!quit) {
        int sig = sigwaitinfo(&mask, &info);
        if (sig == -1) {
            cerr << "\nPro:sigwaitinfo: " << strerror(errno) << endl;
            exit(1);
        }
        else if (sig == SIGINT) {
            quit = 1;
        }
        else if (sig == SIGUSR1){
            cout << "\nPro:Value " << info.si_value.sival_int << endl;
            sprintf(src_queue_name, "/queue_prod%d", info.si_value.sival_int);
            sprintf(dst_queue_name, "/queue_cons%d", info.si_value.sival_int);
            queue_prod_proc = mq_open(src_queue_name, O_RDWR | O_NONBLOCK);

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
            if(msg_proc == -1)
            {
                cerr << "\nPro:Error Receiving" << strerror(errno) << endl;
                exit(1);
            }

            cout << "\nPro:msg_pross " << msg_pross.bytes << endl;

            //temp_msg_pross=msg_pross;
            cout <<"\nPro:Number of bytes: "<< sizeof(msg_pross.bytes)<<endl;
            mq_close(queue_prod_proc);


            //chacha.crypt(&temp_msg_pross.bytes[0], temp_msg_pross.bytes.size());
            chacha.crypt(&msg_pross.bytes[0], msg_pross.numb_bytes_read);

            cout << "\nPro:msg_pross post crypt " << msg_pross.bytes << endl;
            queue_cons_proc = mq_open(dst_queue_name , O_RDWR | O_NONBLOCK);

            if(queue_cons_proc == (mqd_t)-1)
            {
                cerr << "\nPro:In mq_open()" << strerror(errno) << endl;
                exit(1);
            }

            mq_send(queue_cons_proc,reinterpret_cast<const char*>(&msg_pross),sizeof(msg_pross),0);

            mq_close(queue_cons_proc);
        }
    }
    pthread_exit(NULL);
}
