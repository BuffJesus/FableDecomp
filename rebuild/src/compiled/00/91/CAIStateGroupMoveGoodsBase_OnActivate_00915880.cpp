#include "engine/CAIStateGroup_MoveGoodsBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MoveGoodsBase_Methods : CAIStateGroup_MoveGoodsBase {
    void M();
};
void CAIStateGroup_MoveGoodsBase_Methods::M(){ this->DroppingGoods = 0; }