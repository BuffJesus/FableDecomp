struct CScriptThingEntity;
struct MapTreeNode;

struct DoorMap {
    MapTreeNode* LowerBound(int* keyPtr);  // thiscall: ecx=this, stack arg
};

extern CScriptThingEntity* __fastcall GetEnt(void* subobj);

bool __fastcall IsOpenDoor(void* thisptr)
{
    char* subobj = (char*)thisptr + 0xc;
    char* ent;

    if (GetEnt(subobj)
        && !(*(unsigned char*)((char*)GetEnt(subobj) + 0x91) & 0x1)
        && (ent = (char*)GetEnt(subobj), (*(unsigned char*)(ent + 0x24) & 0x40)))
    {
        DoorMap* mapObj = (DoorMap*)(ent + 0x44);

        int key = 0x26;
        MapTreeNode* node = mapObj->LowerBound(&key);
        MapTreeNode* endNode = *(MapTreeNode**)((char*)mapObj + 0x4);

        if (node != endNode && *(int*)node <= 0x26)
            ;
        else
            node = endNode;

        char* value = *(char**)((char*)node + 0x4);

        return *(unsigned char*)(value + 0x12) == 0
            && *(unsigned char*)(value + 0x13) == 0
            && *(unsigned char*)(value + 0x10) != 0;
    }
    return false;
}