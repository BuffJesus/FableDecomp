#pragma once
#include "rebuild_abi.h"
class CEngineLandscapeMap;
class CMemoryDataInputStream;
class CEngineStaticMapFileBlockManager;
enum LS_LOADING_STATUS { LS_LOADING_READY=1,LS_LOADING_PENDING=2 };
// Retail allocation is E8 at BDC180; only virtual destruction and method ABI
// are modelled here. Donor F0 field offsets are not asserted for retail.
class CLandscapeBackgroundTreeNode {
public:
    virtual ~CLandscapeBackgroundTreeNode();
    fable_u8 storage[0xe4];
    CLandscapeBackgroundTreeNode(CEngineLandscapeMap*,void*,long);
    void BuildDefaultSection(unsigned long,unsigned long,unsigned long,unsigned long);
    void LoadHeader(CMemoryDataInputStream&);
    LS_LOADING_STATUS UpdateStaticLOD(double,bool,CEngineStaticMapFileBlockManager*);
};
FABLE_STATIC_ASSERT(sizeof(CLandscapeBackgroundTreeNode)==0xe8);
