#pragma once
#include <stddef.h>
#include "core/heap.h"

inline void* operator new(size_t, void* p) noexcept { return p; }
inline void* operator new[](size_t, void* p) noexcept { return p; }
inline void  operator delete(void*, void*) noexcept { }
inline void  operator delete[](void*, void*) noexcept { }

extern "C" void* heap_alloc(struct heap_ctx* heap, size_t size);
extern "C" void  heap_free(struct heap_ctx* heap, void* ptr);

template <typename T>
class heap_allocated {
public:
    template <typename... Args>
    static T* create(struct heap_ctx* heap, Args&&... args) {
        void* mem = heap_alloc(heap, sizeof(T));
        if (!mem) {
            return nullptr;
        }

        T* obj = new (mem) T(static_cast<Args&&>(args)...);

        return obj;
    }

    void drop(struct heap_ctx* heap) {
        T* obj = static_cast<T*>(this);
        obj->~T();
        heap_free(heap, obj);
    }
};