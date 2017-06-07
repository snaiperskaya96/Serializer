//
// Created by Jury Verrigni on 05/06/2017.
//

#include <cstddef>
#include "SBuffer.h"

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

SCompressedBuffer Serializer::Compress(SBuffer &Buffer, la_ssize_t &ErrorCode, std::string *ErrorMessage)
{
    SCompressedBuffer NewBuffer(Buffer.BufferSize);

    char* OutBuffer = (char*) malloc(Buffer.BufferSize + 180); // Let's make some space for the headers
    struct archive* Archive = archive_write_new();
    archive_write_add_filter_bzip2(Archive);
    archive_write_set_format_ustar(Archive);
    size_t CompressedDataSize;
    la_ssize_t FailCode = archive_write_open_memory(Archive, OutBuffer, Buffer.BufferSize + 180, &CompressedDataSize);

    if (FailCode != ARCHIVE_OK) {
        ErrorCode = FailCode;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    struct archive_entry* Entry = archive_entry_new();
    archive_entry_set_pathname(Entry, "data");
    archive_entry_set_size(Entry, (la_int64_t) Buffer.BufferSize);
    archive_entry_set_filetype(Entry, AE_IFREG);
    archive_entry_set_perm(Entry, 0666);
    FailCode = archive_write_header(Archive, Entry);

    if (FailCode != ARCHIVE_OK) {
        ErrorCode = FailCode;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    FailCode = archive_write_data(Archive, Buffer.Buffer, Buffer.BufferSize);
    archive_write_finish_entry(Archive);
    archive_entry_free(Entry);

    if (FailCode < 0) {
        ErrorCode = FailCode;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    archive_write_close(Archive);
    archive_write_free(Archive);

    NewBuffer.Buffer = realloc(NewBuffer.Buffer, CompressedDataSize + sizeof(size_t));
    memcpy(NewBuffer.Buffer, &Buffer.BufferSize, sizeof(size_t));
    memcpy((char*) NewBuffer.Buffer + sizeof(size_t), OutBuffer, CompressedDataSize);
    NewBuffer.BufferSize = CompressedDataSize;

    free(OutBuffer);
    return NewBuffer;
}

struct SBuffer Serializer::Decompress(struct SCompressedBuffer &Buffer, la_ssize_t &ErrorCode, std::string *ErrorMessage)
{
    SBuffer NewBuffer;
    struct archive* Archive  = archive_read_new();
    archive_read_support_filter_all(Archive);
    archive_read_support_format_raw(Archive);
    la_ssize_t ReadStatus = archive_read_open_memory(Archive, Buffer.Buffer, Buffer.BufferSize);
    if (ReadStatus != ARCHIVE_OK) {
        ErrorCode = ReadStatus;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    struct archive_entry* ArchiveEntry;
    ReadStatus = archive_read_next_header(Archive, &ArchiveEntry);
    if (ReadStatus != ARCHIVE_OK) {
        ErrorCode = ReadStatus;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    char* OutBuffer = (char*) archive_entry_size(ArchiveEntry);

    la_ssize_t Size = archive_read_data(Archive, OutBuffer, Buffer.BufferSize);
    if (Size < 0) {
        ErrorCode = Size;
        const char* Err = archive_error_string(Archive);
        if (Err != NULL) ErrorMessage->assign(Err);
        return NewBuffer;
    }

    archive_read_free(Archive);

    NewBuffer.BufferSize = (size_t) Size;
    NewBuffer.SerializationIndex = (size_t) Size;
    NewBuffer.Buffer = OutBuffer;

    return NewBuffer;
}

void Serializer::ReserveSpace(struct SBuffer &Buffer, size_t Size)
{
    if((Buffer.SerializationIndex + Size) > Buffer.BufferSize) {
        Buffer.Buffer = realloc(Buffer.Buffer, Buffer.SerializationIndex + Size);
        Buffer.BufferSize += Size;
    }
}