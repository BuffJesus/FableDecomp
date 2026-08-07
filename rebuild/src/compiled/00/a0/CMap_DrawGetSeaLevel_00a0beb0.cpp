#include "rebuild_abi.h"
// CMap::DrawGetSeaLevel @ 0x00a0beb0
// fld [ecx+0x2c]; ret  -> return this->f2c (float)
struct CMap { char pad00[0x2c]; float m_seaLevel; float DrawGetSeaLevel(); };
float CMap::DrawGetSeaLevel(){ return m_seaLevel; }