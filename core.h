#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED
#include "btree/btree.h"
#include "storage/link.h"

typedef struct {
    char *dbName;
    BTree tIndex;
} Database;

typedef struct {    //响应结构
    int code;   //错误代码
    char msg[1024]; //错误信息
    void* pData;    //携带信息
} Response;

Response Set(BTree* tIndex,char* key,void* data);
Response Get(BTree* tIndex,char* key);
Response Delete(BTree* tIndex,char *key);
Database* CreateDB(char* dbName);
Database* SwitchDB(char* dbName);
Database* DropDB(char* dbName);
char* ListDB();
#endif // CORE_H_INCLUDED
