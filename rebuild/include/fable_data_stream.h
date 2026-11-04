#pragma once
#include "fable_base_class.h"
// Retail CDataStream vtable 0129A69C: deleting destructor, then three pure slots.
class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream() {}
    virtual void SetPosition(unsigned long) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
};
