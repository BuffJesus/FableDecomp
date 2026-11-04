#include <stdio.h>
#include <string.h>
#include "fable_system_manager.h"
CDriveManager::~CDriveManager(){}
int main(){CDriveManager p;void* vtable=*(void**)&p;CSystemManager* cases[]={0,(CSystemManager*)0x1234,(CSystemManager*)0x4321,0};for(int i=0;i<4;++i){if(p.Initialise(cases[i])!=kSystemCodeSuccess||((CSystemManager**)&p)[1]!=cases[i]||*(void**)&p!=vtable)return 1;}puts("DRIVE_INIT_PASS");return 0;}
