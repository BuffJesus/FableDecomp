#include <stdio.h>

struct C3DVector { float x, y, z; };

struct FrontendInterpOut {
    C3DVector a;   // +0x00
    C3DVector b;   // +0x0c
    C3DVector c;   // +0x18
    C3DVector d;   // +0x24
};

// Stub externs so the test links standalone.
float __cdecl Frontend_GetInterpTime_0042dbfa() { return 10.0f; }
float g_FrontendInterpScale_0042dbfa = 2.0f;

struct CNewFrontendGameComponent {
    unsigned char m_Pad0000[0xb8];
    float         m_StartTime;                 // +0xb8
    unsigned char m_Padbc[0xcc - 0xbc];
    C3DVector     m_cc;                         // +0xcc

    void UpdateInterpolation(FrontendInterpOut* out);
};

void CNewFrontendGameComponent::UpdateInterpolation(FrontendInterpOut* out)
{
    C3DVector v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = (Frontend_GetInterpTime_0042dbfa() - this->m_StartTime)
          * g_FrontendInterpScale_0042dbfa;

    out->a = v;
    out->b = this->m_cc;
    out->c = v;
    out->d = this->m_cc;
    this->m_cc = v;
}

int main()
{
    CNewFrontendGameComponent obj;
    obj.m_StartTime = 3.0f;
    obj.m_cc.x = 7.0f; obj.m_cc.y = 8.0f; obj.m_cc.z = 9.0f;

    FrontendInterpOut out;
    out.a.x = out.a.y = out.a.z = -1.0f;
    out.b.x = out.b.y = out.b.z = -1.0f;
    out.c.x = out.c.y = out.c.z = -1.0f;
    out.d.x = out.d.y = out.d.z = -1.0f;

    obj.UpdateInterpolation(&out);

    // t = (10.0 - 3.0) * 2.0 = 14.0 -> v = {0,0,14}
    const float ez = 14.0f;
    int ok =
        out.a.x == 0.0f && out.a.y == 0.0f && out.a.z == ez &&
        out.b.x == 7.0f && out.b.y == 8.0f && out.b.z == 9.0f &&
        out.c.x == 0.0f && out.c.y == 0.0f && out.c.z == ez &&
        out.d.x == 7.0f && out.d.y == 8.0f && out.d.z == 9.0f &&
        obj.m_cc.x == 0.0f && obj.m_cc.y == 0.0f && obj.m_cc.z == ez;

    if (ok) {
        printf("OK_0x0042dbfa\n");
        return 0;
    }
    printf("FAIL a=(%f,%f,%f) b=(%f,%f,%f) mcc=(%f,%f,%f)\n",
        out.a.x, out.a.y, out.a.z, out.b.x, out.b.y, out.b.z,
        obj.m_cc.x, obj.m_cc.y, obj.m_cc.z);
    return 1;
}