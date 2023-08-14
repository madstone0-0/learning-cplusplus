#include <cstddef>
#include <cstdio>
#include <new>

struct Bucket {
    const static size_t data_size{4096};
    std::byte data[data_size];
};

struct LargeBucket {
    const static size_t data_size{1'048'576};
    std::byte data[data_size];
};

struct Heap {
    void* allocate(size_t bytes) {
        if (bytes > Bucket::data_size && bytes > LargeBucket::data_size) throw std::bad_alloc{};
        for (size_t i{}; i < n_heap_buckets; i++) {
            if (bytes < 4096 && !bucket_used[i]) {
                bucket_used[i] = true;
                return &buckets[i].data;
            }

            if (bytes > 4096 && !large_bucket_used[i]) {
                large_bucket_used[i] = true;
                return &largeBuckets[i].data;
            }
        }
        throw std::bad_alloc{};
    }

    void free(void* p) {
        for (size_t i{}; i < n_heap_buckets; i++) {
            if (buckets[i].data == p) {
                bucket_used[i] = false;
                return;
            }

            if (largeBuckets[i].data == p) {
                large_bucket_used[i] = false;
                return;
            }
        }
    }

    static const size_t n_heap_buckets{10};
    Bucket buckets[n_heap_buckets]{};
    LargeBucket largeBuckets[n_heap_buckets]{};
    bool bucket_used[n_heap_buckets]{};
    bool large_bucket_used[n_heap_buckets]{};
};

Heap heap;

void* operator new(size_t n_bytes) { return heap.allocate(n_bytes); }

void operator delete(void* p) { return heap.free(p); }

int main() {
    printf("Buckets:   %p\nLarge Buckets:   %p\n", heap.buckets, heap.largeBuckets);

    auto breakfast = new unsigned int{0xC0FFEE};
    auto dinner = new unsigned int{0xDEADBEEF};
    auto large = new unsigned char[5120];

    printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
    printf("Dinner:    %p 0x%x\n", dinner, *dinner);

    delete breakfast;
    delete dinner;
    delete[] large;

    // try {
    //     while (true) {
    //         new char;
    //         printf("Allocated a char.\n");
    //     }
    // } catch (const std::bad_alloc&) {
    //     printf("std::bad_alloc caught.\n");
    // }

    return 0;
}
