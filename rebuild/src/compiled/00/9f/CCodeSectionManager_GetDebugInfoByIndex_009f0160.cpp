// CCodeSectionManager::GetDebugInfoByIndex @ 0x009F0160
// (Ego_r ?GetDebugInfoByIndex@CCodeSectionManager@@QAE?AUCDebugInfo@1@J@Z).
// Walks the handle-ordered section table to the index'th entry and returns its
// description by value. The unit keeps CDebugInfo's implicit destructor out of
// line (0x009F00F0, after its default constructor 0x009F00D0).
#include "engine/CCodeSectionManager.h"
CCodeSectionManager::CDebugInfo CCodeSectionManager::GetDebugInfoByIndex(long index)
{
    _STL::map<unsigned long, CSectionDesc>::iterator it = SectionDesc.begin();
    while (it != SectionDesc.end() && index > 0)
    {
        ++it;
        --index;
    }
    CDebugInfo info;
    info.Loaded = it->second.Loaded;
    info.ShortName = it->second.ShortName;
    info.LongName = it->second.LongName;
    info.Size = it->second.Size;
    return info;
}
