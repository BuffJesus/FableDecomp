// GetSoundPair @ 008561a0  (_global, __fastcall)
// Retail: struct-returning forwarder. Hidden return-buffer ptr arrives in ecx
// (1st __fastcall reg), the CThingCreatureBase* in edx (2nd reg), the CCharString*
// on the stack; ret 4. It forwards to virtual slot 89 (0x164/4), passing the SAME
// return buffer straight through (NRVO), then returns the buffer pointer in eax.
//
// Modeled byte-exactly as a free __fastcall that:
//   - takes the return buffer as an explicit first pointer (-> ecx),
//   - the object as second (-> edx),
//   - the CCharString on the stack,
//   - calls the void slot-89 virtual to fill the buffer in place,
//   - returns the buffer pointer.
// This is the only shape VC7.1 forwards without materializing a temp copy.

class CCharString;

struct CSoundPair {
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

struct CThingCreatureBase {
    virtual void s0(CSoundPair*, CCharString*);
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    // slot 89 = 0x164/4
    virtual void GetSoundPairVirtual(CSoundPair* out, CCharString* s);
};

CSoundPair* __fastcall GetSoundPair(CSoundPair* ret, CThingCreatureBase* c, CCharString* s)
{
    c->GetSoundPairVirtual(ret, s);
    return ret;
}