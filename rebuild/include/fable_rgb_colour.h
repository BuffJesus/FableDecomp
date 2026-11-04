#pragma once

// CRGBColour (Ego_r.pdb: a 4-byte union, struct { uchar B, G, R, A; }).
// FableWin.pdb ctors: ??0CRGBColour@@QAE@XZ, (EEE), (EEEE), copy.
// The (r, g, b, a) ctor is inline in retail: CNewFrontendGameComponent::Shutdown
// 0x0042EBB6 stores R, G, B, then A into a stack colour.
class CRGBColour
{
public:
    CRGBColour()
    {
    }

    // FableWin ??0CRGBColour@@QAE@EEE@Z (opaque); inline in retail
    // (CNewFrontendGameComponent::Draw 0x0042DF9E stores R, G, B, then A=0xFF).
    CRGBColour(unsigned char r, unsigned char g, unsigned char b)
    {
        R = r;
        G = g;
        B = b;
        A = 0xFF;
    }

    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    // FableWin's copy ctor: byte by byte in field order (B, G, R, A), as
    // CDataOutputStream::WriteColour 0x00993F30 copies its argument. The
    // compiler-generated copy would be one dword move.
    CRGBColour(const CRGBColour& other)
    {
        B = other.B;
        G = other.G;
        R = other.R;
        A = other.A;
    }

    // ?GetD3DCOLOR@CRGBColour@@QBEKXZ: the whole BGRA dword (Ego_r's union
    // view); inline in retail CDisplayManager::ClearRenderTarget 0x009BE420.
    unsigned long GetD3DCOLOR() const
    {
        return *reinterpret_cast<const unsigned long*>(this);
    }

    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
};

typedef char FableCRGBColourSizeMustBe4[sizeof(CRGBColour) == 4 ? 1 : -1];

// CRGBFloatColour (Ego_r.pdb: R, G, B, A floats, 16 bytes). The (r, g, b, a)
// ctor ??0CRGBFloatColour@@QAE@MMMM@Z is inline in retail
// (CNewFrontendGameComponent::InitialiseEngine 0x0042E204 stores four fld1s).
class CRGBFloatColour
{
public:
    CRGBFloatColour() {}
    CRGBFloatColour(float r, float g, float b, float a)
        : R(r), G(g), B(b), A(a)
    {
    }

    float R;
    float G;
    float B;
    float A;
};

typedef char FableCRGBFloatColourSizeMustBe16[sizeof(CRGBFloatColour) == 16 ? 1 : -1];
