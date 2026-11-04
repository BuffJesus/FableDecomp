// CResourceList::~CResourceList() @ 0x009FC5C0.
#include "fable_resource_list.h"

// Nothing of its own to free: the sentinel's destructor takes it off its
// list (it is never on one) and the failure-handler base is empty.
CResourceList::~CResourceList()
{
}
