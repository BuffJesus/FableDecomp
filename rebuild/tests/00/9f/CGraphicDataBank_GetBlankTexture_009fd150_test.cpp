#include <cstdio>
struct CTexture; enum ETextureType {};
struct CGraphicDataBank { CTexture* GetBlankTexture(ETextureType t); };
CTexture* CGraphicDataBank::GetBlankTexture(ETextureType t){ return (CTexture*)((char*)this + (int)t*8 + 0x2b4); }
int main(){ CGraphicDataBank o; char* base=(char*)&o;
 if((char*)o.GetBlankTexture((ETextureType)3)!=base+3*8+0x2b4){ std::printf("009fd150_TEST FAIL\n"); return 1;}
 std::printf("009fd150_TEST PASS\n"); return 0;}