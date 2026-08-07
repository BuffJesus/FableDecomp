#include "rebuild_abi.h"
// C3DMeshFileGroupChunk::GetList @ 0x00abc420
struct Vec32 { void *m_begin; void *m_end; };
struct C3DMeshFileGroupChunk { char pad[0x10]; Vec32 v; void *GetList(); };
void *C3DMeshFileGroupChunk::GetList()
{
    Vec32 *pv = &v;
    if ((unsigned)(((char*)pv->m_end - (char*)pv->m_begin) >> 5) > 0)
        return pv->m_begin;
    return pv;
}