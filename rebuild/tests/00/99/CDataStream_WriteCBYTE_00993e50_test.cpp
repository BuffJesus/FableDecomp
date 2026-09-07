#include <stdio.h>
struct CDataStream {
    virtual void VSlot0() = 0;
    virtual void VSlot1() = 0;
    virtual void VSlot2() = 0;
    virtual void VSlot3() = 0;
    virtual void WriteRaw(const void* pSource, int size) = 0;
    void WriteCBYTE(char value);
};
void CDataStream::WriteCBYTE(char value) { WriteRaw(&value, 1); }

static int g_size = -1;
static char g_byte = 0;
struct CTestStream : CDataStream {
    void VSlot0() {}
    void VSlot1() {}
    void VSlot2() {}
    void VSlot3() {}
    void WriteRaw(const void* pSource, int size) {
        g_size = size;
        g_byte = *(const char*)pSource;
    }
};

int main() {
    CTestStream stream;
    stream.WriteCBYTE('Q');
    if (g_size != 1) return 1;
    if (g_byte != 'Q') return 2;
    printf("WRITE_CBYTE PASS\n");
    return 0;
}