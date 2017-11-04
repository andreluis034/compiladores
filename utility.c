#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "list.h"

List* skipLevel = EMPTY_LIST;

void pushIgnoredLevel(int level)
{
    skipLevel = prependToListInt(skipLevel, level);
}

int popIgnoredLevel()
{
    int ret = peekIntFromList(skipLevel);
    skipLevel = popIntFromList(skipLevel);
    return ret;
}

int hasInt(List* list, int check)
{
    if(list == NULL)
        return 0;
    if(list->Value.number == check)
    {
      //  printf("has %d\n", check);
        return 1;
    }
    return hasInt(list->Next, check);
}

void popLevels(int level)
{
    while(level <= peekIntFromList(skipLevel)) 
        popIgnoredLevel();
    
}

void printPadding(int level, int lastChild)
{
    if(level == 0)
        return;
    popLevels(level);
    if(lastChild)
        pushIgnoredLevel(level-1);
    char buffer[level*4 + 1] ;
    memset(buffer, 0, sizeof(buffer));
    char buffer1[5] = "|   ";
    char buffer2[5] = "|-- ";
    char buffer3[5] = "|__ ";
    char buffer4[5] = "    ";
    char* finalBuffer = (lastChild == 1 ? buffer3 : buffer2);
    int i;
    for(i = 0; i < (level-1) * 4; i += 4)
    {
        if(hasInt(skipLevel, i/4))
            memcpy(&buffer[i], buffer4, 4);
        else
            memcpy(&buffer[i], buffer1, 4);
    }

    memcpy(&buffer[i], finalBuffer, 4);
    printf("%s", buffer);
}


void printKeyValue(char* prefix, char* suffix, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("%s, %s\n", prefix, suffix);
}