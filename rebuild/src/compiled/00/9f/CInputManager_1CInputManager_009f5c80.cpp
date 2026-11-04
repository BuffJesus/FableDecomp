// CInputManager::~CInputManager (retail 0x009F5C80) and, as its companion, the
// scalar deleting destructor in CInputManager's vtable slot 0 (retail 0x009F6FC0,
// RTTI .?AVCInputManager@@ vtable 0x0129C79C). The body is empty: the members
// (PackageFile, LoadedEventPackages, PJoysticks, PKeyboard, PMouse, EventStore,
// ProcessedEvents) are destroyed by the compiler in reverse declaration order.
#include "fable_input_manager.h"

CInputManager::~CInputManager()
{
}
