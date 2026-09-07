#pragma optimize("s",on)
struct Element450b1b { virtual void Destroy(int flags); unsigned char unused04[0xc0]; };
void __fastcall _Destroy(Element450b1b* first,Element450b1b* last,void*){while(first!=last){first->Destroy(0);++first;}}