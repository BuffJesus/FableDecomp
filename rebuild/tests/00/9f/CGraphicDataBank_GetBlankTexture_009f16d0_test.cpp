#include <cstdio>
struct CTexture; enum ETextureType {};
struct CGraphicDataBank { CTexture* GetBlankTexture(ETextureType t); };
CTexture* CGraphicDataBank::GetBlankTexture(ETextureType t){ return (CTexture*)((char*)this + (int)t*40 + 8); }
int main(){ CGraphicDataBank o; char* base=(char*)&o;
 if((char*)o.GetBlankTexture((ETextureType)3)!=base+3*40+8){ std::printf("009f16d0_TEST FAIL\n"); return 1;}
 std::printf("009f16d0_TEST PASS\n"); return 0;}