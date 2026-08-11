#include <cstdio>

struct CThingInner {
    bool alive;
    virtual bool v00(); virtual bool v01(); virtual bool v02(); virtual bool v03();
    virtual bool v04(); virtual bool v05(); virtual bool v06(); virtual bool v07();
    virtual bool v08(); virtual bool v09(); virtual bool v10(); virtual bool v11();
    virtual bool v12(); virtual bool v13(); virtual bool v14(); virtual bool v15();
    virtual bool v16(); virtual bool v17(); virtual bool v18(); virtual bool v19();
    virtual bool v20(); virtual bool v21(); virtual bool v22(); virtual bool v23();
    virtual bool v24(); virtual bool v25(); virtual bool v26(); virtual bool v27();
    virtual bool v28(); virtual bool v29(); virtual bool v30(); virtual bool v31();
    virtual bool v32(); virtual bool v33(); virtual bool v34(); virtual bool v35();
    virtual bool v36(); virtual bool v37(); virtual bool v38(); virtual bool v39();
    virtual bool v40(); virtual bool v41(); virtual bool v42(); virtual bool v43();
    virtual bool v44(); virtual bool v45(); virtual bool v46(); virtual bool v47();
    virtual bool v48(); virtual bool v49(); virtual bool v50(); virtual bool v51();
    virtual bool v52(); virtual bool v53(); virtual bool v54(); virtual bool v55();
    virtual bool v56(); virtual bool v57(); virtual bool v58(); virtual bool v59();
    virtual bool v60(); virtual bool v61(); virtual bool v62(); virtual bool v63();
    virtual bool v64(); virtual bool v65(); virtual bool v66(); virtual bool v67();
    virtual bool v68(); virtual bool v69(); virtual bool v70(); virtual bool v71();
    virtual bool v72(); virtual bool v73(); virtual bool v74();
    virtual bool IsThingAlive() { return alive; }
};
bool CThingInner::v00(){return false;} bool CThingInner::v01(){return false;}
bool CThingInner::v02(){return false;} bool CThingInner::v03(){return false;}
bool CThingInner::v04(){return false;} bool CThingInner::v05(){return false;}
bool CThingInner::v06(){return false;} bool CThingInner::v07(){return false;}
bool CThingInner::v08(){return false;} bool CThingInner::v09(){return false;}
bool CThingInner::v10(){return false;} bool CThingInner::v11(){return false;}
bool CThingInner::v12(){return false;} bool CThingInner::v13(){return false;}
bool CThingInner::v14(){return false;} bool CThingInner::v15(){return false;}
bool CThingInner::v16(){return false;} bool CThingInner::v17(){return false;}
bool CThingInner::v18(){return false;} bool CThingInner::v19(){return false;}
bool CThingInner::v20(){return false;} bool CThingInner::v21(){return false;}
bool CThingInner::v22(){return false;} bool CThingInner::v23(){return false;}
bool CThingInner::v24(){return false;} bool CThingInner::v25(){return false;}
bool CThingInner::v26(){return false;} bool CThingInner::v27(){return false;}
bool CThingInner::v28(){return false;} bool CThingInner::v29(){return false;}
bool CThingInner::v30(){return false;} bool CThingInner::v31(){return false;}
bool CThingInner::v32(){return false;} bool CThingInner::v33(){return false;}
bool CThingInner::v34(){return false;} bool CThingInner::v35(){return false;}
bool CThingInner::v36(){return false;} bool CThingInner::v37(){return false;}
bool CThingInner::v38(){return false;} bool CThingInner::v39(){return false;}
bool CThingInner::v40(){return false;} bool CThingInner::v41(){return false;}
bool CThingInner::v42(){return false;} bool CThingInner::v43(){return false;}
bool CThingInner::v44(){return false;} bool CThingInner::v45(){return false;}
bool CThingInner::v46(){return false;} bool CThingInner::v47(){return false;}
bool CThingInner::v48(){return false;} bool CThingInner::v49(){return false;}
bool CThingInner::v50(){return false;} bool CThingInner::v51(){return false;}
bool CThingInner::v52(){return false;} bool CThingInner::v53(){return false;}
bool CThingInner::v54(){return false;} bool CThingInner::v55(){return false;}
bool CThingInner::v56(){return false;} bool CThingInner::v57(){return false;}
bool CThingInner::v58(){return false;} bool CThingInner::v59(){return false;}
bool CThingInner::v60(){return false;} bool CThingInner::v61(){return false;}
bool CThingInner::v62(){return false;} bool CThingInner::v63(){return false;}
bool CThingInner::v64(){return false;} bool CThingInner::v65(){return false;}
bool CThingInner::v66(){return false;} bool CThingInner::v67(){return false;}
bool CThingInner::v68(){return false;} bool CThingInner::v69(){return false;}
bool CThingInner::v70(){return false;} bool CThingInner::v71(){return false;}
bool CThingInner::v72(){return false;} bool CThingInner::v73(){return false;}
bool CThingInner::v74(){return false;}

struct CScriptThing {
    void*        pad0;
    CThingInner* thing;
    virtual bool IsAlive() const;
};

bool CScriptThing::IsAlive() const {
    CThingInner* t = thing;
    if (t == 0)
        return false;
    if (t->IsThingAlive())
        return true;
    return false;
}

int main() {
    CThingInner liveT; liveT.alive = true;
    CThingInner deadT; deadT.alive = false;

    CScriptThing st; st.pad0 = 0;

    st.thing = 0;
    bool r_null = st.IsAlive();

    st.thing = &liveT;
    bool r_live = st.IsAlive();

    st.thing = &deadT;
    bool r_dead = st.IsAlive();

    if (!r_null && r_live && !r_dead) {
        printf("ISALIVE_OK\n");
        return 0;
    }
    printf("ISALIVE_FAIL n=%d l=%d d=%d\n", r_null, r_live, r_dead);
    return 1;
}