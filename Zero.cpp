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

void Zero::ProcessMaps() {
    std::filesystem::path p("/proc/" + std::to_string(this->pid) + "/maps");
    std::ifstream ifs(p);
    std::stringstream stream;

    if (ifs)
        stream << ifs.rdbuf();

    std::string line;
    std::string sub;

    while (getline(stream, line)) {
        size_t endpos;
        mapInfo t;
        t.start = std::stoull(line, &endpos, 16); /* 8 + 8 */

        sub = line.substr(endpos + 1);

        t.end = std::stoull(sub, &endpos, 16);
        t.size = t.end - t.start;

        sub = sub.substr(endpos + 20);
        std::stoi(sub, &endpos, 10);
        sub = sub.substr(endpos);

        for (int i = 0; i < sub.length(); i++) {
            if ((sub.at(i) != ' ' && sub.at(i) != '\n')) {
                sub = sub.substr(i);
                break;
            }
        }
        size_t pos = sub.rfind('/');
        if (pos == -1)
            continue;

        sub = sub.substr(pos + 1);
        t.name = sub;

        this->mapData.push_back(t);
    }
    printf("Finished processing maps!\n");
}

mapInfo Zero::FindModuleInfo(const char *name) {
    for (mapInfo &current : mapData) {
        if (current.name == name)
            return current;
    }

    return mapInfo{};
}

uintptr_t Zero::FindPattern(const char *pattern, const char *mask, const char *module, const char* name) {
    char buffer[0x500];
    mapInfo current = this->FindModuleInfo(module);

    size_t readLength = strlen(mask);
    size_t chunkSize = sizeof(buffer);
    size_t chunkCount = 0;
    size_t totalSize = current.size;

    while (totalSize) {
        size_t readSize = (totalSize < chunkSize) ? totalSize : chunkSize;
        uintptr_t readAdress = current.start + (chunkCount * chunkSize);
        bzero(buffer, chunkSize);
        if (this->ReadBuffer(readAdress, buffer, readSize)) {
            for (size_t it = 0; it < readSize; it++) {
                size_t matchCount = 0;
                while (buffer[it + matchCount] == pattern[matchCount] || mask[matchCount] != 'x') {
                    matchCount++;
                    if (matchCount == readLength) {
                        return readAdress + it;
                    }
                }
            }
        }
        chunkCount++;
        totalSize -= readSize;
    }
    printf("Unable to find pattern! %s\n", name);
    return NULL;
}

uintptr_t Zero::GetCallAddress(uintptr_t ptr) {
    int offset = 0;
    this->ReadBuffer(ptr + 1, &offset, sizeof(int));
    return offset + ptr + 5;
}