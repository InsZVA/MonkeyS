#include "storage.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#define free(a) printf("free %x %d\n",a,__LINE__);free(a);
FreeList freeList;
void* GetFree(unsigned int);

void InsertFree(unsigned n,void *pData) //向2^n*seg区插入一段空闲空间
{
    if(n > MAX_FREE_PART)
    {
        //free(pData);
        return;
    }
    LinkNode* t = &freeList.partner[n];
    LinkNode* parent = t;
    t = t->next;
    while(t && t->pData < pData)            // 寻找相邻块并合并
    {
        if(t->pData + (SEGMENT_SIZE << n) == pData)
        {
            parent->next = t->next;
            //printf("combine");
            InsertFree(n+1,t->pData);   //Combine
            free(t);
            return;
        }
        parent = t;
        t = t->next;
    }
    if(t && t->pData == pData + (SEGMENT_SIZE << n))
    {
        //printf("combine");
        InsertFree(n+1,pData);  //合并并插入上个链表
    }
    else
    {
        if(!t)
        {
            LinkNode* p = malloc(sizeof(LinkNode));
            p->next = NULL;
            p->pData = pData;
           parent->next = p;
        }
        else
        {
            LinkNode* p = malloc(sizeof(LinkNode));
            p->next = t->next;
            p->pData = pData;
            t->next = p;
        }
    }
    return;
}

void* Splice(unsigned int  n)   //分裂节点并返回分裂出的一个节点的指向内存，另一个节点插入链表相应位置
{
    if(n > MAX_FREE_PART)   //请求大于最大支持的内存块
    {
        LinkNode* p = &freeList.partner[MAX_FREE_PART - 1]; //是否是最大的块内存不足
        if(p->next) //最大的块还有剩余～说明请求的内存太大了，不支持
        {
            fprintf(stderr,"Max support memory part is 2 ^ %d!\n",MAX_FREE_PART);
            exit(-1);
        }
        //分裂超出最大块出现在最大块已经不足的情况下，那么返回新申请的最大块给他即可
        return malloc(SEGMENT_SIZE << MAX_FREE_PART);
    }
    else
    {
        void* pBig = GetFree(n);    //索取n的空间并分裂
        void* pHalf = pBig + (SEGMENT_SIZE << (n-1));
        InsertFree(n-1,pHalf);
        return pBig;
    }
}

void* GetFree(unsigned int n)   //向空闲链表索取n*segment的空间
{
    LinkNode* p = &freeList.partner[n];
    if(p->next) //第一个节点就有空间
    {
        LinkNode *tn = p->next;
         void *t = tn->pData;
        p->next = p->next->next;    //取出并返回
        free(tn);
        return t;
    }
    else
    {
        return Splice(n+1); //要求分裂更大的块
    }
}

void* InsertToFreeList(unsigned int size,void * pData)   //使用空闲空间链表的一小段空间
{
    unsigned int n = size / SEGMENT_SIZE;
    unsigned int i = 0;
    while(n)
    {
        n >>= 1;
        i++;
    }
    void *p = GetFree(i);   //获取空间
    memset(p,i,1);  //空间第一个字节写入空间大小，为了垃圾回收
    memcpy(p+1,pData,size); //其余空间写入数据
    return p;
}

void Free(void* pData)
{
    unsigned int n = *((char*)pData);   //读出第一个字节表示的该段内存大小
    InsertFree(n,pData);    //回收内存
}

void InitStorage()
{
    //TODO:初始化一些小的内存片段供使用
}
