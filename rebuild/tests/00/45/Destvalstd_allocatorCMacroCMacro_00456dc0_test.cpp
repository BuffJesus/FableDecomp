#include <cstdio>

static int gorder[8];
static int gn = 0;

extern "C" void __fastcall sub_A(void* p){ gorder[gn++] = 0x64; }
extern "C" void __fastcall sub_B(void* p){ gorder[gn++] = 0x54; }
extern "C" void __fastcall sub_C(void* p){ gorder[gn++] = 0x4c; }
extern "C" void __fastcall sub_Base(void* p){ gorder[gn++] = 0; }

void __fastcall Dest_val(char* self)
{
    sub_A(self + 0x64);
    sub_B(self + 0x54);
    sub_C(self + 0x4c);
    sub_Base(self);
}

int main(){
    char buf[0x100];
    Dest_val(buf);
    if (gn==4 && gorder[0]==0x64 && gorder[1]==0x54 && gorder[2]==0x4c && gorder[3]==0){
        std::printf("_Dest_val_std_00456dc0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}