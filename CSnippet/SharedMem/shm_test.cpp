#include <sys/shm.h>
#include <iostream>

const int BUFSIZE = 1024;

int main() {
    int shmId;
    char * shmAddr;

    // Create
    if ((shmId = shmget(IPC_PRIVATE, BUFSIZE, 0666)) < 0) {
        std::cout << "shmget error" << std::endl;
        return -1;
    } else {
        std::cout << "shmget succ " << shmId << std::endl;
    }
    system("ipcs -m");

    // load
    if ((shmAddr = static_cast<char *> (shmat(shmId, 0, 0))) < (char*) 0) {
        std::cout << "shmget error" << std::endl;
        return -1;
    } else {
        std::cout << "attach succ" << std::endl;
    }
    system("ipcs -m");

    if (shmdt(shmAddr) < 0) {
        std::cout << "detach " << std::endl;
        return -1;
    } else {
        std::cout << "deleted shared memory" << std::endl;
    }
    system("ipcs -m");

    // Delete
    if(shmctl(shmId, IPC_RMID, 0) < 0) {
        std::cout << "shmctl error" << std::endl;
        return -1;
    }
    
    std::cout << "release shared memory\n";
    return 0;
}

