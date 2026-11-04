#include "fable_graphic_info.h"
template<class T> inline void CCountedPointer<T>::ReleaseData(){Release();Info=0;Data=0;}
bool CGraphicInfoFrame::CFrameData::StopLoading(){if(!FileData.IsNull()){if(!FileData->IsFinished())goto stillLoading;FileData.ReleaseData();LoadingManagedTexture.Release();}unsigned long frameSize;if(Frame.FrameData)frameSize=Frame.FrameData->Sizeof()+4;else frameSize=4;unsigned long size=sizeof(*this)+frameSize;if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();SetResourceSize(size);return true;stillLoading:return false;}
