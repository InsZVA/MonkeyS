#ifndef LINK_H_INCLUDED
#define LINK_H_INCLUDED

typedef struct _LinkNode{
    void* pData;    //链表内容
    struct _LinkNode * next;    //下一个节点
} LinkNode;

#endif // LINK_H_INCLUDED
