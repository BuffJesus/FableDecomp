// CCountedPointer<CSymbolMap>::Reset(CSymbolMap*) @ 0x009B1F50 (explicitly
// instantiated in DefinitionManager.cpp; DeleteData is 0x009B0E90).
#include "fable_counted_pointer.h"
#include "fable_symbol_map.h"

// Drop the current reference (deleting the map with its last owner), then
// take sole ownership of the new one.
template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
    {
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
    }
}

template class CCountedPointer<CSymbolMap>;
