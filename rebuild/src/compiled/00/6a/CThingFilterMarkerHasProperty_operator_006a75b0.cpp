// CThingFilter_MarkerHasProperty::operator() @ 006a75b0
// return (marker->virt48() & this->m_mask) != 0;

struct CThingMarker {
    virtual int v00();  virtual int v01();  virtual int v02();  virtual int v03();
    virtual int v04();  virtual int v05();  virtual int v06();  virtual int v07();
    virtual int v08();  virtual int v09();  virtual int v10();  virtual int v11();
    virtual int v12();  virtual int v13();  virtual int v14();  virtual int v15();
    virtual int v16();  virtual int v17();  virtual int v18();  virtual int v19();
    virtual int v20();  virtual int v21();  virtual int v22();  virtual int v23();
    virtual int v24();  virtual int v25();  virtual int v26();  virtual int v27();
    virtual int v28();  virtual int v29();  virtual int v30();  virtual int v31();
    virtual int v32();  virtual int v33();  virtual int v34();  virtual int v35();
    virtual int v36();  virtual int v37();  virtual int v38();  virtual int v39();
    virtual int v40();  virtual int v41();  virtual int v42();  virtual int v43();
    virtual int v44();  virtual int v45();  virtual int v46();  virtual int v47();
    virtual int GetProperties(); // slot 48 -> [vtbl+0xc0]
};

struct CThingFilter_MarkerHasProperty {
    int m_mask;   // +0x00
    bool operator()(CThingMarker* marker);
};

bool CThingFilter_MarkerHasProperty::operator()(CThingMarker* marker)
{
    int mask = this->m_mask;
    return (marker->GetProperties() & mask) != 0;
}