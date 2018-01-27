#include <sys/shm.h>
#include <iostream>
#include <sys/ipc.h>
#include <unistd.h>

class Data {
    public:
        float data_rh;
        float data_t;
};

const int KEY = 8888;

int main() {
    void *shm = (void*) 0;

    int shmId;
    Data* data;

    shmId = shmget((key_t)KEY, sizeof(Data), 0666|IPC_CREAT);
    if (shmId < 0) {
        std::cout << "shmget error" << errno << std::endl;
        return -1;
    } else {
        std::cout << "shmget succ " << shmId << std::endl;
    }

    shm = shmat(shmId, (void*)0 , 0);
    if (shm == (void*)-1) {
        std::cout << "shmat error" << std::endl;
        return -1;
    }
    data = (Data* )shm;

    while (1) {
        data->data_rh = rand() % 100;
        data->data_t = rand() % 100;
        std::cout << data->data_rh << "\t" << data->data_t << std::endl;
        sleep(2);
    }
    return 0;
    
}
