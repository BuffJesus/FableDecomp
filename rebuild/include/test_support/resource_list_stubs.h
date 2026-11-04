#pragma once

// Link bodies for fixtures whose leaf constructs or destroys a
// CResourceList-derived object (CDefinitionManager, CResourceBank, ...)
// without testing the list itself. Include this in exactly one fixture TU.
//
// The list's own bodies are the real landed reconstructions:
//   CResourceList::CResourceList          0x009FC570
//   CResourceList::~CResourceList         0x009FC5C0
//   CResourceList::HandleFailedAllocation 0x009FC540
// Only the callees below them are fixture scaffolding (not reconstruction):
// RemoveResource 0x009FC020 and UnloadNextResource 0x009FC170 are counted,
// never exercised. The fixture itself defines
// CIVCountedPointeeBase::DeleteCIVCountedPointeeBase (0x0042D930), which the
// CResource vtable references, so it can observe pointee deletes.
#include "fable_resource_list.h"

#include "../../src/compiled/00/9f/CResourceList_CResourceList_009fc570.cpp"
#include "../../src/compiled/00/9f/CResourceList_Destructor_009fc5c0.cpp"
#include "../../src/compiled/00/9f/CResourceList_HandleFailedAllocation_009fc540.cpp"

static int g_resourceListStubRemoveCalls;
static int g_resourceListStubUnloadCalls;

void CResourceList::RemoveResource(CResource&) { ++g_resourceListStubRemoveCalls; }
bool CResourceList::UnloadNextResource(unsigned long) { ++g_resourceListStubUnloadCalls; return false; }
