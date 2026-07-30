// CIndexBuffer ctor @ 0x0046fece
// 8bc1 mov eax,ecx ; 33c9 xor ecx,ecx ; 8908 mov [eax],ecx ;
// 894804 mov [eax+4],ecx ; 894808 mov [eax+8],ecx ; c3 ret
struct CIndexBuffer {
    void* a;
    void* b;
    void* c;
    CIndexBuffer();
};

CIndexBuffer::CIndexBuffer() {
    a = 0;
    b = 0;
    c = 0;
}