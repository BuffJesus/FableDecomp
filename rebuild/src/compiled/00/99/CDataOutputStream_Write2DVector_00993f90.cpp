#include "engine/CDataOutputStream.h"

struct C2DVector { float X; float Y; };
struct CDataOutputStreamMethods : CDataOutputStream { virtual void Method0(); virtual void Method1(); virtual void Method2(); virtual void Method3(); virtual void WriteBytes(const void* data, int size); void Write2DVector(const C2DVector& value); };

void CDataOutputStreamMethods::Write2DVector(const C2DVector& value)
{
    float component = value.X;
    WriteBytes(&component, sizeof(component));
    component = value.Y;
    WriteBytes(&component, sizeof(component));
}