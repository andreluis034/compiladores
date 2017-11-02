#include <stdio.h>
#include <string.h>
#include "utility.h"
void printPadding(int level, int lastChild)
{
    if(level == 0)
        return;
    char buffer[level*4 + 1] ;
    memset(buffer, 0, sizeof(buffer));
    char buffer1[6] = "|   ";
    char buffer2[6] = "|-- ";
    char buffer3[6] = "|__ ";
    char* finalBuffer = (lastChild == 1 ? buffer3 : buffer2);
    int i;
    for(i = 0; i < (level-1) * 4; ++i)
    {
        buffer[i] = buffer1[i%4];
    }

    memcpy(&buffer[i], finalBuffer, 4);
    printf("%s\n", buffer);
}


void printKeyValue(char* prefix, char* suffix, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("%s, %s\n", prefix, suffix);
}