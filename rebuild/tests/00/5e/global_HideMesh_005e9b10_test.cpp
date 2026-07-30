#include <stdio.h>

struct CMeshEntity {
    virtual void v00()=0;  virtual void v01()=0;  virtual void v02()=0;  virtual void v03()=0;
    virtual void v04()=0;  virtual void v05()=0;  virtual void v06()=0;  virtual void v07()=0;
    virtual void v08()=0;  virtual void v09()=0;  virtual void v10()=0;  virtual void v11()=0;
    virtual void v12()=0;  virtual void v13()=0;  virtual void v14()=0;  virtual void v15()=0;
    virtual void v16()=0;  virtual void v17()=0;  virtual void v18()=0;  virtual void v19()=0;
    virtual void v20()=0;  virtual void v21()=0;  virtual void v22()=0;  virtual void v23()=0;
    virtual void v24()=0;  virtual void v25()=0;  virtual void v26()=0;  virtual void v27()=0;
    virtual void SetVisible(int bVisible)=0;   // slot 28 => byte 0x70
};

void __fastcall HideMesh(CMeshEntity* self)
{
    self->SetVisible(0);
}

static int g_called = 0;
static int g_arg    = -999;

struct CTestMesh : public CMeshEntity {
    void v00(){} void v01(){} void v02(){} void v03(){}
    void v04(){} void v05(){} void v06(){} void v07(){}
    void v08(){} void v09(){} void v10(){} void v11(){}
    void v12(){} void v13(){} void v14(){} void v15(){}
    void v16(){} void v17(){} void v18(){} void v19(){}
    void v20(){} void v21(){} void v22(){} void v23(){}
    void v24(){} void v25(){} void v26(){} void v27(){}
    void SetVisible(int bVisible) { g_called++; g_arg = bVisible; }
};

int main()
{
    CTestMesh obj;
    HideMesh(&obj);   // self passed in ecx via __fastcall/__fastcall

    if (g_called == 1 && g_arg == 0) {
        printf("OK_0x005e9b10\n");
        return 0;
    }
    printf("FAIL called=%d arg=%d\n", g_called, g_arg);
    return 1;
}