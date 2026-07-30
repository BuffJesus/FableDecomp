struct CNavigationLayer {
    int   f00;
    int   f04;
    int   f08;
    char  pad0c;
    char  f0d;
    char  pad0e;
    char  pad0f;
    int   f10;
    int   f14;
    int   f18;
    char  pad1c;
    char  f1d;
};

CNavigationLayer* __fastcall CNavigationLayer_ctor(CNavigationLayer* thisp) {
    thisp->f00 = 0;
    thisp->f04 = 0;
    thisp->f08 = 0;
    thisp->f0d = 0;
    thisp->f10 = 0;
    thisp->f14 = 0;
    thisp->f18 = 0;
    thisp->f1d = 0;
    return thisp;
}