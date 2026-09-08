#include "engine/CDefinitionManager.h"

struct CDefClassBase;

long __stdcall ValidateClassIndex(CDefinitionManager* mgr, long classIndex);

const CDefClassBase* __fastcall GetPDefFromClassIndex(CDefinitionManager* self, long classIndex)
{
    long idx = ValidateClassIndex(self, classIndex);
    if (idx < 0)
        return (const CDefClassBase*)idx;
    const CDefClassBase* const* classArray =
        static_cast<const CDefClassBase* const*>(self->m_classArray);
    return classArray[idx];
}
