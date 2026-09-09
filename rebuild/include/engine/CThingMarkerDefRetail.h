#ifndef FABLE_ENGINE_CTHINGMARKERDEFRETAIL_H
#define FABLE_ENGINE_CTHINGMARKERDEFRETAIL_H

// Retail Fable.exe layout. Ego_r.pdb describes a 0x8c-byte object whose
// derived fields begin at +0x74; retail GetSizeofClass, Transfer, and Copy
// independently prove a 0x88-byte object with those fields shifted to +0x70.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* source) = 0;
};

class CThingBaseDefRetail : public CDefClassBase {
public:
    void CopyBase(const CDefClassBase* source);
    unsigned char _baseData[0x70 - sizeof(void*)];
};

struct CEngineGraphicRetailPayload {
    unsigned char bytes[0x0e];
};

class CThingMarkerDef : public CThingBaseDefRetail {
public:
    CEngineGraphicRetailPayload Graphic; // +0x70; retail Copy copies 14 bytes
    unsigned char _graphicTail[2];       // +0x7e; not copied by retail Copy
    unsigned long TriggerType;           // +0x80 ESwitchTriggerType
    unsigned long Properties;            // +0x84

    virtual void Copy(const CDefClassBase* source);
};

FABLE_STATIC_ASSERT(sizeof(CThingMarkerDef) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThingMarkerDef, Graphic) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThingMarkerDef, TriggerType) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThingMarkerDef, Properties) == 0x84);

#endif
