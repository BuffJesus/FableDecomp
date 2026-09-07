#pragma optimize("s",on)
#include <cstdlib>
struct Payload{unsigned long first,second;};void* __stdcall BinarySearchTreeNode_CreateCopy(Payload* input){void* allocation=std::malloc(0x18);Payload* payload=(Payload*)((unsigned char*)allocation+0x10);if(payload)*payload=*input;return allocation;}