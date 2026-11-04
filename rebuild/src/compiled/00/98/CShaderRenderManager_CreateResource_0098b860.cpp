#include <new>
#include "fable_shader_render_manager.h"
#include "fable_shader_resource.h"
#include "fable_stlport_vector_impl.h"
template<> CCountedPointer<CShaderResource>::CCountedPointer(CShaderResource* data) : Data(data), Info(0)
{
    if(data) Info=new CCPPointerInfo(&DeleteData,data);
}
template<> CCountedPointer<CShaderResource>::CCountedPointer(const CCountedPointer<CShaderResource>& other) : Data(other.Data), Info(other.Info)
{
    if(Info) ++Info->RefCount;
}
CCountedPointer<CShaderResource> CShaderRenderManager::CreateResource()
{
    CCountedPointer<CShaderResource> resource(new CShaderResource);
    resource->Initialise(*this);
    ResourceManagers.push_back(resource);
    return resource;
}
