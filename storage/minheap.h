#ifndef MINHEAP_H_INCLUDED
#define MINHEAP_H_INCLUDED

typedef struct KV {
    unsigned long long key, value;
}KV;
typedef struct MinHeap {
    KV *data;   //数据数组地址
    unsigned long long size;    //堆大小
} MinHeap;


#endif // MINHEAP_H_INCLUDED
