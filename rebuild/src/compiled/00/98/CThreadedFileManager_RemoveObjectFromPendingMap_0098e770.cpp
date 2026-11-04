#include "fable_threaded_file_object.h"

void CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject* object) {
    const unsigned long device = object->PFile->deviceId_;
    CPendingObjectSortKey key;
    key.Object = object;
    _STL::map<CPendingObjectSortKey, CFileObject*>::iterator& current =
        DeviceQueue[device][object->Desc.Priority].CurrentPosition;
    if (current == DeviceQueue[device][object->Desc.Priority].Objects.find(key))
        ++current;
    DeviceQueue[device][object->Desc.Priority].Objects.erase(key);
}
