#include <stdio.h>
struct CTexture { unsigned char m_storage[8]; };
struct CGraphicDataBank { char a[0x1C]; unsigned int index; char* storage; };
struct CGraphicInfoFrame { char a[0x0C]; CGraphicDataBank* bank; CTexture* GetBlankTexture() const; };
CTexture* CGraphicInfoFrame::GetBlankTexture() const {
 return reinterpret_cast<CTexture*>(bank->storage + 0x2B4) + bank->index; }
int main() { char storage[0x400]; CGraphicDataBank bank; bank.index=2; bank.storage=storage;
 CGraphicInfoFrame frame; frame.bank=&bank;
 if(frame.GetBlankTexture()!=reinterpret_cast<CTexture*>(storage+0x2B4)+2) return 1;
 printf("GET_BLANK_TEXTURE PASS\n"); return 0; }