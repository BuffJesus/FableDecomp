#include <new>
#include "fable_engine.h"
namespace _STL {
template<> void vector<CCamera>::_M_insert_overflow(CCamera*,const CCamera&,const __false_type&,size_t,bool);
}
#include "fable_stlport_vector_impl.h"
void CEngine::CutsceneAddCameraAngle(const CCamera& camera) { PreloadCameras.push_back(camera); }
