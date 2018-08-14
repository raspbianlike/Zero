#ifndef ZERO_ZERO_HPP
#define ZERO_ZERO_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <sys/uio.h>

class Zero {
public:

    pid_t pid;

    Zero();

    ~Zero();

    template<class cData>
    inline void ReadBuffer(uintptr_t address, cData *out, size_t size) {
        iovec local_v{out, size};
        iovec remote_v{reinterpret_cast<void *>(address), size};
        long rB = process_vm_readv(this->pid, &local_v, 2, &remote_v, 1, 0);
        if (rB != size) {
            std::cout << "Read invalid amount of bytes from " << std::hex << address << "! Reading failed!" << std::endl;
        }
    }

    template<class cData>
    inline void WriteBuffer(uintptr_t address, cData *in, size_t size) {
        iovec local_v{in, size};
        iovec remote_v{reinterpret_cast<void *>(address), size};
        long wB = process_vm_writev(this->pid, &local_v, 2, &remote_v, 1, 0);

        if (wB != size) {
            std::cout << "Wrote invalid amount of bytes to " << std::hex << address << "! Writing failed!" << std::endl;
        }

    }

    void Process(const char *processName);
};


#endif //ZERO_ZERO_HPP
