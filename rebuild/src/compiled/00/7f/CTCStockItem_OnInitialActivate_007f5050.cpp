// CTCStockItem_Methods::OnInitialActivate  @ 0x007f5050
// push esi; mov esi,ecx; call 0x7f4fc0; test eax,eax; je L; mov al,[esi+0xc];
// test al,al; jne R; L: mov byte[esi+0xc],0; R: ret

#include "engine/CTCStockItem.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTCStockItem_Methods : CTCStockItem {
    // 0x0c

    int Check();
    // direct member call -> 0x7f4fc0 (out-of-line)
    void OnInitialActivate();
};

void CTCStockItem_Methods::OnInitialActivate()
{
    if (this->Check() == 0 || this->ForSale == 0)
        this->ForSale = 0;
}