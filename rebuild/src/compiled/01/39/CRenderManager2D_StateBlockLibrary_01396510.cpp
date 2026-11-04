#include "fable_display_viewport.h"
static CStateBlockFunctionSold Sold;
static CStateBlockFunctionAlphaAdditive Additive;
static CStateBlockFunctionAlphaMultiply Multiply;
static CStateBlockFunctionAlphaBoolean Boolean;
static CStateBlockFunctionAlphaText Text;
CStateBlockFunctionBase* CRenderManager2D::LibraryStateBlockFunctions[7]={0,&Sold,&Additive,&Multiply,&Boolean,&Text,0};
