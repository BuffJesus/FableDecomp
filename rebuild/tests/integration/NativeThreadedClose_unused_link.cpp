#include "fable_gfuninitialise.h"
#include "fable_threaded_file.h"
#include "test_support/base_class_stubs.h"
// The newly recovered destructor is present in the real Close TU. These child
// definitions satisfy its cold link dependencies; this EXE is never executed.
CWideString::~CWideString() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
// Exact typed child only for unexecuted link controls; not recovered ownership.
void CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile*) {}
// Link-only backing alias, NOT a production global/class construction claim.
extern "C" { unsigned char UnusedManagerStorage[0x178]; }
