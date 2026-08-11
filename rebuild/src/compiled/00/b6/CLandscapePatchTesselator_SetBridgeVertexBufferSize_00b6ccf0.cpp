// CLandscapePatchTesselator::SetBridgeVertexBufferSize @ 0x00b6ccf0
// void __fastcall (CLandscapePatchTesselator*, long)
//
// Retail body:
//   mov edx,[esp+4]     ; long n
//   sub esp,0x18        ; 24-byte local temp
//   lea eax,[esp]       ; &temp
//   push eax            ; out
//   push edx            ; n
//   add ecx,0x14        ; &this->m_bridge  (this for callee)
//   call 0xb6cd10       ; m_bridge.SetSize(n, &temp)  (temp discarded)
//   add esp,0x18
//   ret 4

struct Buf24 {
    char d[24];
};

struct BridgeBuffer {
    // out-pointer method (explicit args): void SetSize(long, Buf24*)
    void SetSize(long n, Buf24* out);
};

struct CLandscapePatchTesselator {
    char m_pad[0x14];        // 0x00 .. 0x13
    BridgeBuffer m_bridge;   // 0x14

    void SetBridgeVertexBufferSize(long n);
};

void CLandscapePatchTesselator::SetBridgeVertexBufferSize(long n)
{
    Buf24 tmp;
    m_bridge.SetSize(n, &tmp);
}