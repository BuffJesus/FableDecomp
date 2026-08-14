// CGraphicDataBank::PrepareDataForInclusion (0x009fd990)
extern "C" void __cdecl free(void*);
struct CCountedPtr { long count; void (__fastcall *release)(void*); void* obj; };
struct CGraphicDataBank; struct CWorldMap; struct CArrayStr; struct CDataOutputStream;
void __fastcall PrepareDataForInclusion(CGraphicDataBank* self, void*, CWorldMap* a1, long a2, const CArrayStr* a3, long a4, CCountedPtr* p, CDataOutputStream* out1, CDataOutputStream* out2){ if (p){ if (--p->count == 0){ p->release(p->obj); free(p); } } }