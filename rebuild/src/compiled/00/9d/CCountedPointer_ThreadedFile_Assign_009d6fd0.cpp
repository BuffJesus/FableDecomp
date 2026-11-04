#include "fable_threaded_file.h"
CCountedPointer<CThreadedFile>& CCountedPointer<CThreadedFile>::operator=(const CCountedPointer& other)
{
    CThreadedFile* data=other.data_;
    CCPPointerInfo* control=other.control_;
    if(control_!=control){
        if(control_){
            --control_->RefCount;
            if(!control_->RefCount){control_->DeleteFunc(control_->Data);delete control_;}
        }
        data_=data;control_=control;
        if(control_)++control_->RefCount;
    }
    return *this;
}
