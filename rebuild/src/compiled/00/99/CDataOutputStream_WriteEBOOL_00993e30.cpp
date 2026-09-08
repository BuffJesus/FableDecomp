#include "engine/CDataOutputStream.h"

struct CDataOutputStream_Methods : CDataOutputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Write(void* data, int size);
    void WriteEBOOL(bool value);
};

void CDataOutputStream_Methods::WriteEBOOL(bool value) {
    unsigned char serializedValue = value ? 1 : 0;
    Write(&serializedValue, 1);
}
