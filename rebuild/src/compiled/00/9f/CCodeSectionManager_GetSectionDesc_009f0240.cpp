#include "engine/CCodeSectionManager.h"
CCodeSectionManager::CSectionDesc& CCodeSectionManager::GetSectionDesc(unsigned long handle)
{
 _STL::map<unsigned long,CSectionDesc>::iterator it=SectionDesc.find(handle);
 if(it==SectionDesc.end()) {
  static CSectionDesc desc;
  desc.Loaded=false;
  desc.Size=0;
  return desc;
 }
 return it->second;
}