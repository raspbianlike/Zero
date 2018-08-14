#include "Zero.hpp"

using namespace std;

int main() {
    Zero z;

    string sampleString("I am a string!");
    uintptr_t samplePointer = (uintptr_t) &sampleString;

    z.Process("Zero");

    char readString[128];
    uintptr_t tempPointer;

    /* --------------------------------------------------------- */

    z.ReadBuffer(samplePointer, &tempPointer, sizeof(tempPointer));
    z.ReadBuffer(tempPointer, &readString, sizeof(readString));

    cout << "Before-write (0x" << hex << samplePointer << ") = " << readString << endl;

    /* --------------------------------------------------------- */

    string writeString("Me too!");

    z.WriteBuffer(tempPointer, const_cast<char *>(writeString.c_str()), sizeof(writeString.c_str()));
    z.WriteBuffer(samplePointer, &writeString, sizeof(writeString));
    z.WriteBuffer(samplePointer, &tempPointer, sizeof(tempPointer));


    cout << "After-write  (0x" << samplePointer << ") = " << sampleString << endl;

    /* --------------------------------------------------------- */

    return EXIT_SUCCESS;
}