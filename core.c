#include "btree/btree.h"
#include "storage/storage.h"
#include "core.h"
#include <stdio.h>
#include <string.h>

LinkNode dbLink;

Response Set(BTree* tIndex,char *key,void* data)
{
    unsigned long long hash = GetHash(key);
    void* pData = InsertToFreeList(strlen(data) + 1,data);
    int p;
    BTNode *node = BTree_search(*tIndex,hash,&p);
    if(node != NULL)    //如果已存在则删除
    {
        Free(node->pRecord[p]);
        BTree_remove(tIndex,hash);
    }
    BTree_insert(tIndex,hash,pData);
    //printf("Set %s(Hash key:%llu) = \"%s\" on data %x\n",key,hash,data,pData);
    Response r;
    r.code = 0;
    sprintf(r.msg,"Set %s(Hash key:%llu) = \"%s\" on data %x\n",key,hash,data,pData);
    r.pData = pData + 1;    //第一个字节储存块大小，不返回
    return r;
}

Response Get(BTree* tIndex,char *key)
{
    Response r;
    unsigned long long hash = GetHash(key);
    int p;
    BTNode *node = BTree_search(*tIndex,hash,&p);
    if(node)
    {
        //printf("Find %s(Hash key:%d) on index %x with data \"%s\"\n",key,hash,p,node->pRecord[p] + 1);
        r.code = 0;
        sprintf(r.msg,"Find %s(Hash key:%d) on index %x with data \"%s\"\n",key,hash,p,node->pRecord[p] + 1);
        r.pData = node->pRecord[p] + 1;     //第一个字节储存块大小，不返回
    }
    else
    {
        printf("Cannot find %s\n",key);
        r.code = 1;
        sprintf(r.msg,"Cannot find %s\n",key);
        r.pData = NULL;
    }
    return r;
}

Response Delete(BTree* tIndex,char *key)
{
    Response r;
    unsigned long long hash = GetHash(key);
    int p;
    BTNode *node = BTree_search(*tIndex,hash,&p);
    if(node != NULL)
    {
        Free(node->pRecord[p]);
        BTree_remove(tIndex,hash);
        //printf("Delete %s(Hash key:%d)\n",key);
        r.code = 0;
        sprintf(r.msg,"Delete %s(Hash key:%d)\n",key);
        r.pData = NULL;
    }
    else
    {
        r.code = 1;
        //printf("Cannot find %s\n",key);
        sprintf(r.msg,"Cannot find %s\n",key);
        r.pData = NULL;
    }
    return r;
}

Database* CreateDB(char* dbName)
{
    Database *d = SwitchDB(dbName);
    if(d)return d;
    Database* p = malloc(sizeof(Database));
    p->dbName = malloc(strlen(dbName) + 1);
    strcpy(p->dbName,dbName);
    p->tIndex = NULL;   //简单清0即为BTree初始化
    LinkNode* newNode = malloc(sizeof(LinkNode));
    newNode->next = dbLink.next;
    newNode->pData = p;
    dbLink.next = newNode;
    //printf("Create db:%s\n",dbName);
    return NULL;
}

Database* SwitchDB(char *dbName)
{
    LinkNode *p = dbLink.next;
    while(p)
    {
        if(strcmp(((Database*)(p->pData))->dbName,dbName) == 0)
        {
            //printf("Switch to db:%s\n",dbName);
            return p->pData;
        }
        p = p->next;
    }
    //printf("Cannot find db:%s\n",dbName);
    return NULL;
}

Database* DropDB(char *dbName)
{
    if(strcmp(dbName,"monkey") == 0)
    {
        //printf("Cannot drop monkey db\n");
        return SwitchDB("monkey");
    }
    LinkNode *p = dbLink.next;
    LinkNode *parent = &dbLink;
    while(p)
    {
         if(strcmp(((Database*)(p->pData))->dbName,dbName) == 0)
        {
            //printf("Drop db:%s\n",dbName);
            parent->next = p->next;                         //删除节点
            BTree_destroy(&((Database*)(p->pData))->tIndex); //删除索引树
            free(((Database*)(p->pData))->dbName);
            free(p->pData);
            free(p);
            return SwitchDB("monkey");
        }
        parent = p;
        p = p->next;
    }
    //printf("Cannot find db:%s",dbName);
    return SwitchDB("monkey");
}

char* ListDB()              //用户需要自行处理掉返回的char*防止内存泄露
{
    char* r = malloc(1024);
    r[0] = 0;
    LinkNode *p = dbLink.next;
    while(p)
    {
        //printf(" %s",((Database*)(p->pData))->dbName);
        sprintf(r," %s",((Database*)(p->pData))->dbName);
        p = p->next;
    }
    printf("\n");
    return r;
}
