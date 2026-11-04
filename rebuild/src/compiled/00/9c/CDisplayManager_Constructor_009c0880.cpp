// Native 009C0880; shared ownership and field order from PDB plus retail.
#include <new>
#include "fable_display_viewport.h"
// The two out-of-line native growth paths are separate recovery units.
namespace _STL {
template<> void vector<CPixelFormat>::_M_insert_overflow(CPixelFormat*, const CPixelFormat&, const __false_type&, size_t, bool);
template<> void vector<ESurfaceMultisampleType>::_M_insert_overflow(ESurfaceMultisampleType*, const ESurfaceMultisampleType&, const __false_type&, size_t, bool);
}
#include "fable_stlport_vector_impl.h"
// Native display construction leaves this parameter record untouched.

CDisplayManager::CDisplayManager(CSystemManager& system)
    : SystemManager(&system), PRenderManager(0), D3D(0), D3DDevice(0),
      BackBufferFormatStorage_(-1), DepthBufferFormatStorage_(-1),
      FrameCounter(0), VsyncCallbackTimer(0), Gamma(1.0f)
{
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(25)));
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(31)));
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(21)));
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(23)));
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(24)));
    BackBufferFormats.push_back(CPixelFormat(static_cast<D3DFORMAT>(22)));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(0));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(2));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(3));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(4));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(5));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(6));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(7));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(8));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(9));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(10));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(11));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(12));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(13));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(14));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(15));
    MultisampleTypes.push_back(static_cast<ESurfaceMultisampleType>(16));
}
