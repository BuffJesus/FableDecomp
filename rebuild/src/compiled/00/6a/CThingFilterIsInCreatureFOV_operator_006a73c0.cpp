#include "rebuild_abi.h"
// CThingFilter::operator() @ 0x006a73c0
struct Inner { void Call(void *arg, unsigned char flag); };
struct CThingFilter { Inner *m_inner; unsigned char m_flag; void operator()(void *arg); };
void CThingFilter::operator()(void *arg){
    m_inner->Call(arg, m_flag);
}