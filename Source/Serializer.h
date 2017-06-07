//
// Created by Jury Verrigni on 01/06/2017.
//

#ifndef LIBKEY_SERIALIZER_H
#define LIBKEY_SERIALIZER_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <archive_entry.h>
#include <archive.h>

class Serializer {
public:
    static void Serialize(struct SBuffer& Buffer, void* Object, size_t Size);

    static void Deserialize(struct SBuffer& Buffer, void* Destination, size_t Size);

    static struct SCompressedBuffer Compress(struct SBuffer& Buffer, la_ssize_t& ErrorCode, std::string* ErrorMessage);

    static struct SBuffer Decompress(SCompressedBuffer &Buffer, la_ssize_t &ErrorCode, std::string *ErrorMessage);

protected:
    static void ReserveSpace(struct SBuffer& Buffer, size_t Size);

};

#endif //LIBKEY_SERIALIZER_H
