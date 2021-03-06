Serializer
===================


Serializer is a small C++ utils that doesn't depend on (almost) any external library.
It can be useful for serializing (oh, really?) data and store it or send it over the network.

It features compressing and decompressing of data using libarchive which is a little of an overkill but features a lot of different options and the code is quite easy to change. I'm planning to add my own compression class though in order to make it as small as possible.

----------


Example
-------------
An example can be found within the Example folder. Will be compiled with the library.
```
#include <iostream>
#include "SBuffer.h"

int main()
{
    SBuffer Buffer;

    const int A = 12345;
    const float B = 44.32;
    const std::string C = "Right";

    // Serialize standard types automagically
    // It consumes slightly more bytes because
    // It needs to store the type size
    Buffer << A;
    Buffer << B;
    Buffer << C;
    // We can store data manually if we already know the size
    const char* D = "Left!";
    Serializer::Serialize(Buffer, (void*) D, 5);

    std::cout << "Serialized Buffer:" << std::endl << std::string((char*) Buffer.Buffer, Buffer.BufferSize) << std::endl;

    la_ssize_t ErrorNo;
    std::string ErrorMessage;
    SCompressedBuffer CompressedBuffer = Serializer::Compress(Buffer, ErrorNo, &ErrorMessage);

    // Doesn't really have enough data to be properly compressed, as a result it will be bigger than the original buffer
    std::cout << "Compressed Buffer:" << std::endl << std::string((char*) CompressedBuffer.Buffer, CompressedBuffer.BufferSize) << std::endl;

    // If we keep the same serialization order,
    // the data will be correct
    int NewA; float NewB; std::string NewC;
    char* NewD = (char*) malloc(5);

    Buffer >> NewA;
    Buffer >> NewB;
    Buffer >> NewC;
    Serializer::Deserialize(Buffer, NewD, 5);

    std::cout << std::endl << std::endl;
    std::cout << "___| Before | After |" << std::endl;
    std::cout << " A |  " << A << " | " << NewA << " |" <<  std::endl;
    std::cout << " B |  " << B << " | " << NewB << " |" <<  std::endl;
    std::cout << " C |  " << C << " | " << NewC << " |" <<  std::endl;
    std::cout << " D |  " << D << " | " << NewD << " |" <<  std::endl;

}
```
