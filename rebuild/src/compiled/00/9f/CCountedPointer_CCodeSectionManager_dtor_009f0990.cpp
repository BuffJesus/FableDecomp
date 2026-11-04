// CCountedPointer<CCodeSectionManager>::~CCountedPointer @ 0x009F0990.
// The CodeSectionManager unit defines the singleton's counted pointer
// (CTBaseSingleton<CCodeSectionManager>::PClass, retail 0x013CAA68); its
// dynamic atexit destructor (0x0122A960, next to GetSectionDesc's `desc`
// atexit 0x0122A940) expands the destructor inline and leaves this COMDAT.
#include "engine/CCodeSectionManager.h"
CCountedPointer<CCodeSectionManager> g_FableCodeSectionManager_013CAA68;
