#pragma once
#include "rebuild_abi.h"
// Ego_r.pdb: four-byte polymorphic interface, Apply is its only virtual slot.
class CStateBlockFunctionBase
{
public:
    virtual void Apply() = 0;
};
FABLE_STATIC_ASSERT(sizeof(CStateBlockFunctionBase)==4);
class CStateBlockFunctionSold : public CStateBlockFunctionBase
{ public: virtual void Apply(); };
class CStateBlockFunctionAlphaAdditive : public CStateBlockFunctionBase
{ public: virtual void Apply(); };
class CStateBlockFunctionAlphaMultiply : public CStateBlockFunctionBase
{ public: virtual void Apply(); };
class CStateBlockFunctionAlphaBoolean : public CStateBlockFunctionBase
{ public: virtual void Apply(); };
class CStateBlockFunctionAlphaText : public CStateBlockFunctionBase
{ public: virtual void Apply(); };
