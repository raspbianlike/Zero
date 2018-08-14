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

    printf("Before-write (%#p) = %s\n", samplePointer, readString);

    /* --------------------------------------------------------- */

    string writeString("Me too!");

    z.WriteBuffer(tempPointer, const_cast<char *>(writeString.c_str()), sizeof(writeString.c_str()));
    z.WriteBuffer(samplePointer, &writeString, sizeof(writeString));
    z.WriteBuffer(samplePointer, &tempPointer, sizeof(tempPointer));


    printf("After-write  (%#p) = %s\n", samplePointer, sampleString.c_str());

    /* --------------------------------------------------------- */

    printf("Finished!\n");

    return EXIT_SUCCESS;
}