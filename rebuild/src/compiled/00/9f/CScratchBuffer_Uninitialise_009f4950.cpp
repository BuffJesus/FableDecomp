#include <new>
#include "fable_system_manager.h"
void CScratchBuffer::Uninitialise(){if(IsValid()&&Buffer){delete[] Buffer;Buffer=0;}CInitBaseClass::Uninitialise();}