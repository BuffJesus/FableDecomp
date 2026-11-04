#include "fable_threaded_file_object.h"
// Descriptor defaults are constructed first; all other scalar fields stay unwritten.
CThreadedFileManager::CFileObject::CFileObject() : Overlapped(0)
{
}
