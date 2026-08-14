#include <stdio.h>
#include <string.h>

// Behaviour model: Copy forwards base-copy then sub-copy(src+0x28).

struct Src { unsigned char bytes[0x40]; };

struct Sub { int val; void Copy(const void* s){ val = *(const int*)s; } };

struct Base { unsigned char base[0x28]; void Copy(const void* s){ memcpy(base, s, 0x28); } };

struct Special : public Base {
    Sub sub;
    void Copy(const void* src){
        Base::Copy(src);
        sub.Copy((const char*)src + 0x28);
    }
};

int main(){
    Src src; for(int i=0;i<0x40;i++) src.bytes[i]=(unsigned char)(i+1);
    Special dst; memset(&dst,0,sizeof(dst));
    dst.Copy(&src);

    int ok = 1;
    for(int i=0;i<0x28;i++) if(dst.base[i]!=(unsigned char)(i+1)) ok=0;
    // sub reads the int at src+0x28 = bytes 0x29,0x2a,0x2b,0x2c => little endian
    int expected = *(const int*)(src.bytes+0x28);
    if(dst.sub.val != expected) ok=0;

    if(ok) printf("COPY_OK\n");
    else printf("COPY_FAIL\n");
    return 0;
}