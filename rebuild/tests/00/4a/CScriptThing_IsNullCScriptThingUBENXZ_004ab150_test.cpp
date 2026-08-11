#include <cstdio>

struct Inner
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76();
    virtual bool v77();
};

struct CScriptThing
{
    virtual bool IsNull() const;
    Inner* m4;
};

// independent reimplementation of the function under test
bool CScriptThing::IsNull() const
{
    Inner* inner = m4;
    if (inner == 0)
        return true;
    if (inner->v77())
        return true;
    return false;
}

// concrete Inner subclasses returning known values
struct InnerTrue : Inner  { bool v77() { return true; } };
struct InnerFalse : Inner { bool v77() { return false; } };

// stub out pure-ish virtuals: give bodies so linkage works
void Inner::v00(){} void Inner::v01(){} void Inner::v02(){} void Inner::v03(){}
void Inner::v04(){} void Inner::v05(){} void Inner::v06(){} void Inner::v07(){}
void Inner::v08(){} void Inner::v09(){} void Inner::v10(){} void Inner::v11(){}
void Inner::v12(){} void Inner::v13(){} void Inner::v14(){} void Inner::v15(){}
void Inner::v16(){} void Inner::v17(){} void Inner::v18(){} void Inner::v19(){}
void Inner::v20(){} void Inner::v21(){} void Inner::v22(){} void Inner::v23(){}
void Inner::v24(){} void Inner::v25(){} void Inner::v26(){} void Inner::v27(){}
void Inner::v28(){} void Inner::v29(){} void Inner::v30(){} void Inner::v31(){}
void Inner::v32(){} void Inner::v33(){} void Inner::v34(){} void Inner::v35(){}
void Inner::v36(){} void Inner::v37(){} void Inner::v38(){} void Inner::v39(){}
void Inner::v40(){} void Inner::v41(){} void Inner::v42(){} void Inner::v43(){}
void Inner::v44(){} void Inner::v45(){} void Inner::v46(){} void Inner::v47(){}
void Inner::v48(){} void Inner::v49(){} void Inner::v50(){} void Inner::v51(){}
void Inner::v52(){} void Inner::v53(){} void Inner::v54(){} void Inner::v55(){}
void Inner::v56(){} void Inner::v57(){} void Inner::v58(){} void Inner::v59(){}
void Inner::v60(){} void Inner::v61(){} void Inner::v62(){} void Inner::v63(){}
void Inner::v64(){} void Inner::v65(){} void Inner::v66(){} void Inner::v67(){}
void Inner::v68(){} void Inner::v69(){} void Inner::v70(){} void Inner::v71(){}
void Inner::v72(){} void Inner::v73(){} void Inner::v74(){} void Inner::v75(){}
void Inner::v76(){}
bool Inner::v77(){ return false; }

int main()
{
    int ok = 1;

    // Case 1: m4 == null  -> IsNull true
    CScriptThing a; a.m4 = 0;
    if (a.IsNull() != true) ok = 0;

    // Case 2: m4->v77() true -> IsNull true
    InnerTrue objTrue;
    Inner* pTrue = &objTrue;
    CScriptThing b; b.m4 = pTrue;
    if (b.IsNull() != true) ok = 0;

    // Case 3: m4->v77() false -> IsNull false
    InnerFalse objFalse;
    Inner* pFalse = &objFalse;
    CScriptThing c; c.m4 = pFalse;
    if (c.IsNull() != false) ok = 0;

    if (ok) printf("ISNULL_OK\n");
    else    printf("ISNULL_FAIL\n");
    return ok ? 0 : 1;
}