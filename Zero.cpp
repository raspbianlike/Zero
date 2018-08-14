#include "Zero.hpp"

Zero::Zero() {
}

Zero::~Zero() {
}

void Zero::Process(const char *processName) {
    DIR *maps = opendir("/proc/");

    if (!maps) {
        std::cout << "Failed to read \"/proc/\" directory! Exit..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    struct dirent *dir;

    while (dir = readdir(maps), dir != nullptr) {

        if (dir->d_type != DT_DIR)
            continue;

        std::string sPid = dir->d_name;
        std::ifstream s("/proc/" + sPid + "/comm");
        std::string comm;
        getline(s, comm);

        if (!strcmp(comm.c_str(), processName)) {
            pid_t tPid = atoi(sPid.c_str());
            std::cout << "Found target process! PID: " << tPid << std::endl;
            this->pid = tPid;
            return;
        }

    }
    std::cout << "Failed to find target process! Exit..." << std::endl;
    std::exit(EXIT_FAILURE);
}