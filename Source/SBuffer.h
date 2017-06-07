//
// Created by Jury Verrigni on 05/06/2017.
//

#ifndef LIBKEY_SBUFFER_H
#define LIBKEY_SBUFFER_H

#include <string>
#include <cstdlib>
#include "Serializer.h"

#define SBUFFER_INITIAL_SIZE 32


struct SAbstractBuffer
{
    void* Buffer;
    size_t BufferSize;

    SAbstractBuffer()
    {
        Buffer = malloc(SBUFFER_INITIAL_SIZE);
        BufferSize = SBUFFER_INITIAL_SIZE;
    }
};

struct SBuffer : public SAbstractBuffer {
    size_t SerializationIndex;
    size_t DeserializationIndex;

    SBuffer() {
        SerializationIndex = 0;
        DeserializationIndex = 0;
    }

    /** A few type specific helpers */
    void operator<<(std::string String)
    {
        unsigned long StringLength = String.size();
        Serializer::Serialize(*this, &StringLength, sizeof(unsigned long));
        Serializer::Serialize(*this, (void*)String.data(), StringLength);
    }

    void operator>>(std::string& String)
    {
        unsigned long StringLength;
        Serializer::Deserialize(*this, &StringLength, sizeof(unsigned long));

        char* CharArray = (char*) malloc(StringLength);
        Serializer::Deserialize(*this, CharArray, StringLength);

        String.assign(CharArray, StringLength);
    }

    template<class T>
    friend SBuffer& operator<<(SBuffer& Buffer, T In)
    {
        Serializer::Serialize(Buffer, &In, sizeof(T));
        return Buffer;
    }

    template<class T>
    friend SBuffer& operator>>(SBuffer& Buffer, T& Out)
    {
        Serializer::Deserialize(Buffer, &Out, sizeof(T));
        return Buffer;
    }
};

struct SCompressedBuffer : public SAbstractBuffer
{
    size_t OriginalSize = 0;

    SCompressedBuffer(size_t NewSize)
    {
        Buffer = realloc(Buffer, NewSize + sizeof(size_t));
        BufferSize = NewSize;
    }

    void Init()
    {
        if (OriginalSize != 0) return;
        memcpy(&OriginalSize, Buffer, sizeof(size_t));
        memcpy(Buffer, (char*) Buffer + sizeof(size_t), BufferSize);
    }
};

#endif //LIBKEY_SBUFFER_H
