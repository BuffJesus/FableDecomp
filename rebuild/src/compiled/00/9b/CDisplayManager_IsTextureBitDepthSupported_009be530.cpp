// CDisplayManager::IsTextureBitDepthSupported  (VC7.1 __fastcall, returns bool)
//
// Walks a global mode/format table (records of stride 0x24 = 9 dwords). The
// exported symbol points at record 0's "key" field; the "flag" and "payload"
// fields sit 4 and 8 bytes behind it. For each record whose flag == 2 and whose
// key matches the requested format, it queries the D3D device (vtable slot 10)
// to test whether that surface/back-buffer combination is supported; a
// non-negative HRESULT means yes. The list terminates when the next record's
// key field is -1.
extern int g_bitDepthTable[];  // symbol == &record0.key

struct CDisplayManager {
    char  pad0[0x54];
    void* dev;                 // +0x54  D3D device wrapper
    char  pad1[0x04];
    int   field5c;             // +0x5c
    int   field60;             // +0x60
    char  pad2[0x1c4 - 0x64];
    int   field1c4;            // +0x1c4
    bool  IsTextureBitDepthSupported(int fmt);
};

bool CDisplayManager::IsTextureBitDepthSupported(int fmt)
{
    if (g_bitDepthTable[0] != -1) {
        int* p = g_bitDepthTable;
        do {
            if (p[-1] == 2 && fmt == p[0]) {
                void*  dev = this->dev;
                void** vt  = *(void***)dev;
                int r = ((int(__stdcall*)(void*,int,int,int,int,int,int))vt[10])(
                    dev, this->field60, this->field5c, this->field1c4, 0, 3, p[-2]);
                if (r >= 0)
                    return true;
            }
            p += 9;
        } while (p[0] != -1);
    }
    return false;
}