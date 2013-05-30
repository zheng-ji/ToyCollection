#include "memo.h"
static const int KBlockSize = 4096;

CMemo::CMemo() {
    alloc_ptr_ = NULL;
    alloc_bytes_remaining_ = 0;
    blocks_memory_ = 0;
}

char * CMemo::AllocateNewBlock(size_t block_bytes) {
    char* result = new char[block_bytes];
    blocks_memory_ += block_bytes;
    blocks_.push_back(result);
    return result;
}

char* CMemo::AllocateFallback(size_t bytes) {
    if (bytes > KBlockSize / 4) {
        char* result = AllocateNewBlock(bytes); 
        return result;
    }
    alloc_ptr_ = AllocateNewBlock(KBlockSize);
    alloc_bytes_remaining_ = KBlockSize;

    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
}

CMemo::~CMemo() {
    for (size_t i = 0; i < blocks_.size();i++)
        delete[] blocks_[i];
}
