#include "core.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
extern LinkNode dbLink;
Database* db;
void* GetFree(unsigned int);
void brenchmark2()
{
for(int i = 0;i < 100000;i++)
    {
   void* p = GetFree(i%13);
   Free(p);
    }
}

void brenchmark()
{
    CreateDB("monkey");
    db = SwitchDB("monkey");
    struct timeval struct_start, struct_end;
    char s[512] = "1232dsazcaffeafa3rfvfsbvq4gfawfaf";
    gettimeofday(&struct_start,NULL);
    long start = ((long)struct_start.tv_sec)*1000 +  (long)struct_start.tv_usec/1000;
    for(int i = 0;i < 100000;i++)
    {
        Set(&db->tIndex,&i,s);
    }
    gettimeofday(&struct_end, NULL);
    long end = ((long)struct_end.tv_sec)*1000 + (long)struct_end.tv_usec/1000;
    printf("Insert 100000 * 1024 within %d\n",end - start);
    char* a;
    gettimeofday(&struct_start,NULL);
    start = ((long)struct_start.tv_sec)*1000 +  (long)struct_start.tv_usec/1000;
    for(int i = 0;i < 100000;i++)
    {
        a = Get(&db->tIndex,&i).pData;
        if(!a)continue;
    }
    gettimeofday(&struct_end, NULL);
    end = ((long)struct_end.tv_sec)*1000 + (long)struct_end.tv_usec/1000;
    printf("%s",a);
    printf("Get 100000 * 1024 within %d\n",end - start);
}

int main(int argc,char** argv)
{
    InitStorage();
    debug();
    //brenchmark();
    //brenchmark2();
}

void debug()
{
    char buffer1[4096],buffer2[4096];
    CreateDB("monkey");
    db = SwitchDB("monkey");
    while(1)
    {
        printf("monkey:%s>",db->dbName);
        scanf("%s",buffer1);
        if(strcmp(buffer1,"set") == 0)
        {
            scanf(" %s %s",buffer1,buffer2);
            Response r = Set(&db->tIndex,buffer1,buffer2);
            printf(r.msg);
        }
        else if(strcmp(buffer1,"get") == 0)
        {
            scanf(" %s",buffer1);
            Get(&db->tIndex,buffer1);
        }
        else if(strcmp(buffer1,"remove") == 0 || strcmp(buffer1,"delete") == 0)
        {
            scanf(" %s",buffer1);
            Delete(&db->tIndex,buffer1);
        }
        else if(strcmp(buffer1,"createdb") == 0)
        {
            scanf(" %s",buffer1);
            Database* d = CreateDB(buffer1);
            if(!d)continue;
            db = d;
        }
        else if(strcmp(buffer1,"switchdb") == 0)
        {
            scanf(" %s",buffer1);
            Database* d = SwitchDB(buffer1);
            if(!d)continue;
            db = d;
        }
        else if(strcmp(buffer1,"dropdb") == 0)
        {
            scanf(" %s",buffer1);
            db = DropDB(buffer1);
        }
        else if(strcmp(buffer1,"listdb") == 0)
        {
            free(ListDB());
        }
        else if(strcmp(buffer1,"exit") == 0)
        {
            return;
        }
    }
}
