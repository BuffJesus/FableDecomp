struct VolObj;
typedef void (__stdcall *VolFn)(VolObj*);

struct VolVtbl
{
    VolFn f0;
    VolFn f1;
    VolFn release;
};

struct VolObj
{
    VolVtbl* vtbl;
};

struct CVolume
{
    int pad0;
    VolObj* m_vol;
    void ReleaseVolume();
};

void CVolume::ReleaseVolume()
{
    VolObj* v = m_vol;
    if (v)
    {
        v->vtbl->release(v);
        m_vol = 0;
    }
}