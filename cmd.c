#include "cmd.h"

CmdList* makeCmdList(Cmd* firstCmd)
{
    return (CmdList*)makeList((void*)firstCmd);
}
CmdList* appendCmd(CmdList* list, Cmd* value) 
{
    return (CmdList*)appendToList(list, (void*)value);
}
CmdList* prependCmd(CmdList* list, Cmd* value) 
{
    return (CmdList*)prependToList(list, (void*)value);    
}

Cmd* getCmd(CmdList* list) 
{
    return (Cmd*)list->Value;
}