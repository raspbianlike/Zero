#include "Zero.hpp"

Zero::Zero() {
}

Zero::~Zero() {
}

void Zero::Process(const char *processName) {

    for (auto &dir : std::filesystem::directory_iterator("/proc/")) {
        if (!dir.is_directory() || !std::isdigit(dir.path().filename().c_str()[0]))
            continue;

        std::string comm;
        getline(std::ifstream(dir.path() / "comm"), comm);

        if (comm == processName) {
            printf("Found target process! PID: %s\n", dir.path().filename().c_str());
            this->pid = atoi(dir.path().filename().c_str());
            return;
        }
    }
    printf("Failed to find target process... Exit...\n");
    std::exit(EXIT_FAILURE);
}