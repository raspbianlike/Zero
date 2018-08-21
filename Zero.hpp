#ifndef ZERO_ZERO_HPP
#define ZERO_ZERO_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <sys/uio.h>
#include <filesystem>

struct mapInfo {
    uintptr_t start = NULL;
    uintptr_t end = NULL;
    size_t size = 0;
    std::string name;
};

class Zero {
public:
    std::vector<mapInfo> mapData;

    pid_t pid;

    Zero();

    ~Zero();

    template<class cData>
    bool ReadBuffer(uintptr_t address, cData *out, size_t size) {
        iovec local_v{out, size};
        iovec remote_v{reinterpret_cast<void *>(address), size};
        long rB = process_vm_readv(this->pid, &local_v, 1, &remote_v, 1, 0);

        if (rB != size) {
            printf("Read invalid amount of bytes from %p! Reading failed!\n", address);
            return false;
        }
        return true;
    }

    template<class cData>
    bool WriteBuffer(uintptr_t address, cData *in, size_t size) {
        iovec local_v{in, size};
        iovec remote_v{reinterpret_cast<void *>(address), size};
        long wB = process_vm_writev(this->pid, &local_v, 2, &remote_v, 1, 0);

        if (wB != size) {
            printf("Wrote invalid amount of bytes to %p! Writing failed!\n", address);
            return false;
        }
        return true;
    }

    void Process(const char *processName);

    void ProcessMaps();

    mapInfo FindModuleInfo(const char *name);

    uintptr_t FindPattern(const char *pattern, const char *mask, const char *module, const char* name);

    uintptr_t GetCallAddress(uintptr_t ptr);
};


#endif //ZERO_ZERO_HPP
