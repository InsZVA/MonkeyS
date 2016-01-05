#include "core.h"
#include <stdio.h>
#include <string.h>
extern LinkNode dbLink;
Database* db;

int main(int argc,char** argv)
{
    InitStorage();
    debug();
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
            Set(&db->tIndex,buffer1,buffer2);
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
