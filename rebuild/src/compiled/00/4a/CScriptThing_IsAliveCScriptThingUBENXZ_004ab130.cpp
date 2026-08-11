// Inner object: has a vtable; slot 75 (offset 0x12c) is a bool()-returning virtual.
struct CThingInner {
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
    virtual bool IsThingAlive(); // slot 75 = offset 0x12c
};

struct CScriptThing {
    // +0 = vtable ptr (polymorphic)
    CThingInner* thing;  // +4
    virtual int IsAlive() const;
};

int CScriptThing::IsAlive() const {
    CThingInner* t = thing;
    if (t != 0 && t->IsThingAlive())
        return 1;
    return 0;
}