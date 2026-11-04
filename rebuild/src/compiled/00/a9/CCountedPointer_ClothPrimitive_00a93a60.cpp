// CCountedPointer<C3DClothPrimitive> COMDATs in the lib_3d_primitive_2 template pool: DeleteData (retail 0x00A93A60, a
// virtual delete of the primitive), the CCountedPointer(T*) constructor (0x00A94D60), the destructor (0x00A94DB0) and
// Reset(T*) (0x00A94DF0). C3DPrimitive2 wraps each cloth primitive it builds (C3DClothPrimitive ctor 0x00AC3E00, vtable
// 0x0129DF74) in a counted pointer whose shared block stores 0x00A93A60.
#include "fable_counted_pointer.h"
#include "fable_3d_cloth_primitive.h"

template<typename T>
CCountedPointer<T>::CCountedPointer(T* data)
{
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
    else
        Info = 0;
}

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
}

template void CCountedPointer<C3DClothPrimitive>::DeleteData(void*);
template CCountedPointer<C3DClothPrimitive>::CCountedPointer(C3DClothPrimitive*);
template CCountedPointer<C3DClothPrimitive>::~CCountedPointer();
template void CCountedPointer<C3DClothPrimitive>::Reset(C3DClothPrimitive*);
