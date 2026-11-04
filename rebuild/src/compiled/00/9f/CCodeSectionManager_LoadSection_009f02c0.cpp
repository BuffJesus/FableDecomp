#include "engine/CCodeSectionManager.h"
void CCodeSectionManager::LoadSection(unsigned long handle) {
 if (handle != (unsigned long)-1 && !GetSectionDesc(handle).Loaded)
  GetSectionDesc(handle).Loaded = true;
}