#include "engine/CEngineWaterRenderer.h"
template<class T> inline void CCountedPointer<T>::Reset(T* data) {
    Release();
    Info=0;
    Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
void CEngineWaterRenderer::CloseStaticMap() {
    if(HaveBackgroundSea) {
        SeaBackgroundVertexBuffer.Uninitialise();
        SeaBackgroundVertexCount=0;
        SeaBackgroundIndexBuffer.Uninitialise();
        SeaBackgroundPolyCount=0;
        HaveBackgroundSea=false;
    }
    if(LoadingSea.Data) {
        CBankFileAsyncData::DiscardAsyncData(LoadingSea);
        LoadingSea.Reset(0);
        LoadingSeaEntry.Reset(0);
    }
    StaticMapBankFile=0;
}
