// Byte-exact reconstruction of CGameCameraManager::GetRenderCamera 0x00697f30
// ?GetRenderCamera@CGameCameraManager@@UBE?AVCCamera@@M@Z
//
// Virtual const member returning CCamera by value. Forwards its hidden
// return-slot (esi = [esp+8]) directly to another thiscall member at 0x6978d0,
// passing (m, &field4, &field3c). Return-slot chaining via copy-elision keeps
// the retail push;call;ret shape (no local temp / rep movsl).

struct CCamera { char data[0x40]; };
struct CFieldA { char pad[4]; };
struct CFieldB { char pad[4]; };

struct CGameCameraManager {
    virtual CCamera GetRenderCamera(float m) const;              // slot -> 0x00697f30
    CCamera Helper6978d0(float m, CFieldA* a, CFieldB* b) const; // 0x006978d0

    // vptr occupies +0x00
    CFieldA field4;          // +0x04
    char    pad8[0x34];      // +0x08 .. +0x3b
    CFieldB field3c;         // +0x3c
};

CCamera CGameCameraManager::GetRenderCamera(float m) const
{
    return Helper6978d0(m, (CFieldA*)&field4, (CFieldB*)&field3c);
}