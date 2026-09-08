#pragma optimize("s",on)
struct C2DBoxF { float TLX; float TLY; float BRX; float BRY; };
struct CTCInventoryBaseRetailRenderBox {
    unsigned char Reserved[0xbc];
    C2DBoxF InventoryAnimRenderBox;
};
void __fastcall SetInventoryAnimRenderBox(CTCInventoryBaseRetailRenderBox* self, void*, const C2DBoxF* box)
{
    C2DBoxF* destination = &self->InventoryAnimRenderBox;
    destination->TLX = box->TLX;
    destination->TLY = box->TLY;
    destination->BRX = box->BRX;
    destination->BRY = box->BRY;
}