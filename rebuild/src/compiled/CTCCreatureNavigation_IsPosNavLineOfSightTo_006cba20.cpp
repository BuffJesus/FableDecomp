struct CPos;
struct CNavHelper { int __fastcall Compute(const CPos* edx, const CPos* p); };
struct CVtbl { bool (__fastcall *m[16])(void*, float arg); };
struct CNavObj { CVtbl* vptr; };
struct CTCCreatureNavigation { void* pad0; CNavHelper* helper; void* pad8; CNavObj* obj; };
bool __fastcall CTCCreatureNavigation_IsPosNavLineOfSightTo(CTCCreatureNavigation* self, void* edxpad, const CPos* pos)
{
    CNavObj* obj = self->obj;
    CVtbl* vt = obj->vptr;
    union { int i; float f; } u;
    u.i = self->helper->Compute(pos, pos);
    return vt->m[10](self->obj, u.f);
}