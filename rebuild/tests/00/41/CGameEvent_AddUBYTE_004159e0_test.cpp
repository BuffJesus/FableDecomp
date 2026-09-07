#include <stdio.h>
struct CMemoryBuffer {
    int lastSize;
    unsigned char lastByte;
    void Copy(int size, const void* pSource);
};
void CMemoryBuffer::Copy(int size, const void* pSource) {
    lastSize = size;
    lastByte = *(const unsigned char*)pSource;
}

struct CGameEvent : CMemoryBuffer {
    void AddUBYTE(unsigned char value);
};
void CGameEvent::AddUBYTE(unsigned char value) { Copy(1, &value); }

int main() {
    CGameEvent event;
    event.lastSize = -1;
    event.lastByte = 0;
    event.AddUBYTE(0xAB);
    if (event.lastSize != 1) return 1;
    if (event.lastByte != 0xAB) return 2;
    printf("ADD_UBYTE PASS\n");
    return 0;
}