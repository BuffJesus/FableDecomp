#pragma optimize("s", on)
#include "fable_resource_list.h"
void CResource::ListRemove() {
 if (!NextResource) return;
 if (PrevResource->NextResource == this && NextResource->PrevResource == this) {
  PrevResource->NextResource = NextResource; NextResource->PrevResource = PrevResource;
 }
 *reinterpret_cast<CResource* volatile*>(&PrevResource) = 0;
 *reinterpret_cast<CResource* volatile*>(&NextResource) = 0;
}
