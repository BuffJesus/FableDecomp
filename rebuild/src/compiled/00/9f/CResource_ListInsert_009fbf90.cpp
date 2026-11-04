#include "fable_resource_list.h"
void CResource::ListInsert(CResource* previous,CResource* next) {
 PrevResource = previous; NextResource = next;
 previous->NextResource = this; NextResource->PrevResource = this;
}
