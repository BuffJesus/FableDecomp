struct CAFile
{
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void Finish(long value);
    virtual void V6();
    virtual long Read();
};

struct CWorldFileLoader
{
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual void Load(CAFile* file, long value);
};

struct CWorld
{
    virtual void V0();
    virtual void FinishLoad();
    long pad[4];
    CWorldFileLoader* loader;

    void LoadWorldFromFile(CAFile* file, long value);
};

extern float CWorldLoadTimer();

void CWorld::LoadWorldFromFile(CAFile* file, long value)
{
    (void)CWorldLoadTimer();
    long result = file->Read();
    loader->Load(file, value);
    file->Finish(result);
    FinishLoad();
    (void)CWorldLoadTimer();
}