#include "engine/CEditWorldMap.h"

struct CEditMapRuntimeEntry {
    unsigned char data[0x48];
};

struct CEditWorldMap_Methods : CEditWorldMap {
    void SetMapAsChanged(int index, char changed);
};

void CEditWorldMap_Methods::SetMapAsChanged(int index, char changed)
{
    CEditMapRuntimeEntry* entries = *(CEditMapRuntimeEntry**)&this->_pad_0x20[0];
    *((char*)&entries[index] + 0x27) = changed;
}
