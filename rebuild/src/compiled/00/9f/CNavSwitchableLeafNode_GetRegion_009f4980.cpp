#include "fable_system_manager.h"
bool CScratchBuffer::Lock(void** buffer){if(!Locked){*buffer=Buffer;Locked=true;return true;}return false;}
