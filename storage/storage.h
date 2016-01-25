#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED
#include "link.h"

#define SEGMENT_SIZE 1024
#define INIT_SEGMENT_NUM 32
#define MAX_FREE_PART 13    //最大2^13大小的块


typedef struct {    //伙伴系统
    LinkNode partner[MAX_FREE_PART];   //2^0~2^13=4096 空头链表
}FreeList;


//初始时初始化INIT_SEGMENT_NUM个段，如果新插入的记录能够放进段中，则放入，每个段最多放一个，如果放不进去，则放入页中

void InitStorage(); //初始化储存引擎
void* InsertToFreeList(unsigned int size,void * pData);  //向储存引擎插入数据
void Free(void* pData); //回收内存

#endif // STORAGE_H_INCLUDED
