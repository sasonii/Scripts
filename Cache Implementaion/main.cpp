#include <iostream>
#include <cstring>

typedef unsigned char uchar;

typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar* block;
} cache_line_t;

typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t** cache;
} cache_t;
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar* start, long int off);
void write_byte(cache_t cache, uchar* start, long int off, uchar new_char);
void print_cache(cache_t cache);

//int main() {
//    int n;
//    printf("Size of data: ");
//    scanf("%d", &n);
//    uchar* mem = (uchar *) malloc(n);
//    printf("Input data >> ");
//    for (int i = 0; i < n; i++)
//        scanf("%hhd", mem + i);
//    int s, t, b, E;
//    printf("s t b E: ");
//    scanf("%d %d %d %d", &s, &t, &b, &E);
//    cache_t cache = initialize_cache(s, t, b, E);
//
//    while (1) {
//        scanf("%d", &n);
//        if (n < 0) break;
//        read_byte(cache, mem, n);
//    }
//
//    puts("");
//    print_cache(cache);
//
//    free(mem);
//}

int main() {
    uchar arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    cache_t cache = initialize_cache(1, 1, 1, 2);
    read_byte(cache, arr, 0);
    read_byte(cache, arr, 1);
    read_byte(cache, arr, 2);
    read_byte(cache, arr, 6);
    read_byte(cache, arr, 7);
    print_cache(cache);
    return 0;
}

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t cache;
    cache.s = s;
    cache.t = t;
    cache.b = b;
    cache.E = E;

    int S = 1 << s;
    int B = 1 << b;

    cache.cache = (cache_line_t**) malloc(S * sizeof(cache_line_t*));
    for (int i = 0; i < S; i++) {
        cache.cache[i] = (cache_line_t*) malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            cache.cache[i][j].block = (uchar*) malloc(B * sizeof(uchar));
            std::memset(cache.cache[i][j].block, 0, B * sizeof(uchar));
        }
    }

    return cache;
}

uchar read_byte(cache_t cache, uchar* start, long int off){
    int S = 1 << cache.s;
    int B = 1 << cache.b;
    long int tag = off >> (cache.s + cache.b);
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1); //TC
    int block_offset = off & ((1 << cache.b) - 1);

    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[set_index][i].valid && cache.cache[set_index][i].tag == tag) {
            cache.cache[set_index][i].frequency++;
            return cache.cache[set_index][i].block[block_offset];
        }
    }

    // Cache miss, fetch from memory
    uchar data = start[off];

    // Find the least frequently used cache line
    int min_frequency = INT_MAX;
    int min_frequency_index = 0;
    for (int i = 0; i < cache.E; i++) {
        if (!cache.cache[set_index][i].valid) {
            min_frequency_index = i;
            break;
        }
        if (cache.cache[set_index][i].frequency < min_frequency) {
            min_frequency = cache.cache[set_index][i].frequency;
            min_frequency_index = i;
        }
    }

    // Update the cache line with the fetched data
    cache.cache[set_index][min_frequency_index].valid = 1;
    cache.cache[set_index][min_frequency_index].frequency = 1;
    cache.cache[set_index][min_frequency_index].tag = tag;
    for (int i = 0; i < B; i++) {
        cache.cache[set_index][min_frequency_index].block[i] = start[(off & ~(B - 1)) + i];
    }

    return data;
}

void write_byte(cache_t cache, uchar* start, long int off, uchar new_char){
    int S = 1 << cache.s;
    int B = 1 << cache.b;
    long int tag = off >> (cache.s + cache.b);
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    int block_offset = off & ((1 << cache.b) - 1);

    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[set_index][i].valid && cache.cache[set_index][i].tag == tag) {
            cache.cache[set_index][i].frequency++;
            cache.cache[set_index][i].block[block_offset] = new_char;
            start[off] = new_char;
            return;
        }
    }

    // Cache miss, fetch from memory
    start[off] = new_char;

    // Find the least frequently used cache line
    int min_frequency = INT_MAX;
    int min_frequency_index = 0;
    for (int i = 0; i < cache.E; i++) {
        if (!cache.cache[set_index][i].valid) {
            min_frequency_index = i;
            break;
        }
        if (cache.cache[set_index][i].frequency < min_frequency) {
            min_frequency = cache.cache[set_index][i].frequency;
            min_frequency_index = i;
        }
    }

    // Update the cache line with the fetched data
    cache.cache[set_index][min_frequency_index].valid = 1;
    cache.cache[set_index][min_frequency_index].frequency = 1;
    cache.cache[set_index][min_frequency_index].tag = tag;
    for (int i = 0; i < B; i++) {
        cache.cache[set_index][min_frequency_index].block[i] = start[(off & ~(B - 1)) + i];
    }
}

void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                   cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}