#include <cstdio>

struct CDataOutputStream {
    virtual void slot0() {}
    virtual void slot1() {}
    virtual void slot2() {}
    virtual void slot3() {}
    virtual void Write(void* buf, unsigned int len);
};

struct CChunkedFileSaver { int dummy; };

struct CChunkedFileRawDataChunk {
    char pad[0x10];
    void* f10;
    unsigned int f14;
    void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver);
};

void CChunkedFileRawDataChunk::WriteToFile(CDataOutputStream& stream,
                                           CChunkedFileSaver& saver)
{
    stream.Write(this->f10, this->f14);
}

static void* g_buf;
static unsigned int g_len;
static CDataOutputStream* g_self;

void CDataOutputStream::Write(void* buf, unsigned int len) {
    g_self = this; g_buf = buf; g_len = len;
}

int main() {
    CDataOutputStream stream;
    CChunkedFileRawDataChunk obj;
    int payload = 0x1234;
    void* ppayload = &payload;
    obj.f10 = ppayload;
    obj.f14 = 777;
    CChunkedFileSaver saver;
    CDataOutputStream* pstream = &stream;

    obj.WriteToFile(stream, saver);

    if (g_self == pstream && g_buf == ppayload && g_len == 777) {
        std::printf("00aa7830_TEST PASS\n");
    } else {
        std::printf("FAIL self=%p buf=%p len=%u\n", (void*)g_self, g_buf, g_len);
    }
    return 0;
}