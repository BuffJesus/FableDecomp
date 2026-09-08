#include "engine/CExpressionSubDef.h"

struct CExpressionSubDef_Methods : CExpressionSubDef {
    void BaseClone(const CExpressionSubDef* source);
};

void __fastcall CExpressionSubDef_Copy(
    CExpressionSubDef_Methods* self, int /*edx*/, const CExpressionSubDef* src) {
    self->BaseClone(src);
    self->ExpressionDef = src->ExpressionDef;
}
