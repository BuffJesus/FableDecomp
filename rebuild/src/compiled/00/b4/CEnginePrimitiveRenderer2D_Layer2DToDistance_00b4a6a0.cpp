struct CEnginePrimitiveRenderer2D { float Layer2DToDistance(int l); };
float CEnginePrimitiveRenderer2D::Layer2DToDistance(int l){ return *(float*)((char*)this + l*4 + 0x1d4); }