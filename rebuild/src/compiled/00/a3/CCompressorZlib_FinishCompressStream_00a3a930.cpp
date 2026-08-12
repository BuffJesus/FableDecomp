// CCompressorZlib::FinishCompressStream  @ 0x00a3a930
// Clears an active zlib deflate stream if one is open.

extern "C" int __cdecl EndDeflateStream(void* strm);

struct CCompressorZlib {
    char pad[0x1c];      // 0x00
    char zstream[0x38];  // 0x1c: z_stream-like block
    char active;         // 0x54: "stream open" flag
    void FinishCompressStream();
};

void CCompressorZlib::FinishCompressStream()
{
    if (this->active) {
        EndDeflateStream(&this->zstream);
        this->active = 0;
    }
}