#include "engine/CCompressorZlib.h"
#include <stdio.h>
int main(){if(CCompressorZlib::GetRequiredWorkspaceSize()!=0)return 1;puts("WORKSPACE_PASS");return 0;}
