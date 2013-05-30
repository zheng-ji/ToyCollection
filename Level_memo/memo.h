#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <vector>
#include <assert.h>
#include <cstddef>

class CMemo {
    public:
        CMemo();
        ~CMemo();

        char * Allocate(size_t bytes);
        size_t MemoryUsage() const {
            return blocks_memory_ + blocks_.capacity() * sizeof(char *);
        };

    private:
        char * alloc_ptr_;
        size_t alloc_bytes_remaining_;
        size_t blocks_memory_;
        std::vector<char *> blocks_;
        
        CMemo(const CMemo &);
        void operator= (const CMemo&);
        char* AllocateFallback(size_t bytes);
        char* AllocateNewBlock(size_t block_bytes);
};

inline char* CMemo::Allocate(size_t bytes) {
    assert(bytes > 0);
    if (bytes <= alloc_bytes_remaining_) {
        char * result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;
        return result;
    }
    return AllocateFallback(bytes);
}

#endif
