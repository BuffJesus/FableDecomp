#include "rebuild_abi.h"
// C3DMeshFileGroupChunk::GetList @ 0x00ac4080 (stride 8, vec at +0x4)
struct Vec8 { void *m_begin; void *m_end; };
struct GroupChunk8 { char pad[0x4]; Vec8 v; void *GetList(); };
void *GroupChunk8::GetList()
{
    Vec8 *pv = &v;
    if ((unsigned)(((char*)pv->m_end - (char*)pv->m_begin) >> 3) > 0)
        return pv->m_begin;
    return pv;
}