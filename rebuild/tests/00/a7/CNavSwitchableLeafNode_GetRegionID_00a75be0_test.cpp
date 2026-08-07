#include "rebuild_abi.h"
#include <cstdio>
struct CNavSwitchableLeafNode { char pad00[0xf]; unsigned char m_disabled; long *m_link; long GetRegionID(); };
long CNavSwitchableLeafNode::GetRegionID(){ if(!m_disabled){ if(m_link) return *m_link; } return 0; }
int main(){
    CNavSwitchableLeafNode n; long id=55;
    n.m_disabled=1; n.m_link=&id;
    if(n.GetRegionID()!=0){std::printf("B1\n");return 1;}
    n.m_disabled=0; n.m_link=0;
    if(n.GetRegionID()!=0){std::printf("B2\n");return 1;}
    n.m_link=&id;
    if(n.GetRegionID()!=55){std::printf("B3\n");return 1;}
    std::printf("REGIONID_OK\n"); return 0;
}