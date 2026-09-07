#pragma optimize("s",on)
struct Element450a90 { virtual void Destroy(int flags); unsigned char unused04[0x50]; };
void __fastcall _Destroy(Element450a90* first,Element450a90* last,void*){while(first!=last){first->Destroy(0);++first;}}