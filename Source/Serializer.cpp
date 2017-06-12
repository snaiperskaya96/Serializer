//
// Created by Jury Verrigni on 05/06/2017.
//

#include <cstddef>
#include "SBuffer.h"
#ifdef WITH_ZLIB
#include <zlib.h>
#endif
void Serializer::Serialize(SBuffer &Buffer, void *Object, size_t Size)
{
    ReserveSpace(Buffer, Size);

    memcpy(((char *)Buffer.Buffer) + Buffer.SerializationIndex, Object, Size);
    Buffer.SerializationIndex += Size;
}

void Serializer::Deserialize(SBuffer &Buffer, void *Destination, size_t Size)
{
    memcpy(Destination, ((char*)Buffer.Buffer) + Buffer.DeserializationIndex, Size);
    Buffer.DeserializationIndex += Size;
}

#ifdef WITH_ZLIB
SCompressedBuffer Serializer::Compress(SBuffer &Buffer)
{
    SCompressedBuffer NewBuffer(0);
    Bytef* TempBuffer = (Bytef*) malloc(Buffer.SerializationIndex * 3);
    uLongf NewSize = Buffer.SerializationIndex * 3;
    compress(TempBuffer, &NewSize, (const Bytef *) Buffer.Buffer, Buffer.SerializationIndex);
    NewBuffer.BufferSize = NewSize;
    NewBuffer.Buffer = (char*) TempBuffer;
    NewBuffer.OriginalSize = Buffer.SerializationIndex;
    NewBuffer.Init();

    return NewBuffer;
}

struct SBuffer Serializer::Decompress(SCompressedBuffer &Buffer)
{
    SBuffer NewBuffer;
    Bytef* TempBuffer = (Bytef*) malloc(Buffer.OriginalSize);
    uLongf NewSize = Buffer.OriginalSize;
    uncompress(TempBuffer, &NewSize, (const Bytef *) Buffer.Buffer, Buffer.BufferSize);
    NewBuffer.BufferSize = NewSize;
    NewBuffer.SerializationIndex = NewSize;
    NewBuffer.Buffer = (char*) TempBuffer;

    return NewBuffer;
}
#endif

void Serializer::ReserveSpace(struct SBuffer &Buffer, size_t Size)
{
    if((Buffer.SerializationIndex + Size) > Buffer.BufferSize) {
        Buffer.Buffer = realloc(Buffer.Buffer, Buffer.SerializationIndex + Size);
        Buffer.BufferSize += Size;
    }
}