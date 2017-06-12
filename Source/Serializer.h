//
// Created by Jury Verrigni on 01/06/2017.
//

#ifndef LIBKEY_SERIALIZER_H
#define LIBKEY_SERIALIZER_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

class Serializer {
public:
    static void Serialize(struct SBuffer& Buffer, void* Object, size_t Size);

    static void Deserialize(struct SBuffer& Buffer, void* Destination, size_t Size);

#ifdef WITH_ZLIB
    static struct SCompressedBuffer Compress(struct SBuffer& Buffer);

    static struct SBuffer Decompress(SCompressedBuffer &Buffer);
#endif

protected:
    static void ReserveSpace(struct SBuffer& Buffer, size_t Size);

};

#endif //LIBKEY_SERIALIZER_H
