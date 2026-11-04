#include <new>
#include "fable_system_manager.h"
bool CScratchBuffer::Initialise(long size){Buffer=0;if(CInitBaseClass::Initialise()){Buffer=new fable_u8[size];if(Buffer){NoBytes=size;Locked=false;return true;}}return false;}