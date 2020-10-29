#include "Common.h"

template <typename T>
class MemoryPool {

    private:
        T* start;
        T* nextFreeSlot;
        T* end; 

        inline constexpr void reallocate(){
            size_t newSize = size_t(end - start) * 2;
            start = (T*)realloc(start, newSize);
            nextFreeSlot = start + newSize/2;
            end = (T*)newSize;
        }

    public:
        MemoryPool(size_t initialCapacity){
            start = (T*)malloc(initialCapacity * sizeof(T));
            nextFreeSlot = start;
            end = start + initialCapacity;
        }

        inline constexpr T* allocate(){
            if(nextFreeSlot == end)
                reallocate();
            return nextFreeSlot++;
        }

        inline constexpr void deallocate(){ /* TODO implement me */ }

        inline constexpr void freePool(){free(start);}

        inline constexpr T* data(){ return start; }

        inline constexpr size_t size(){ return size_t(nextFreeSlot) / sizeof(T); } //TODO may have to subtract 1 from nextFreeSlot

};
