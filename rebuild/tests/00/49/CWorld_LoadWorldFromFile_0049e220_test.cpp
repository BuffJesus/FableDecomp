#include <cstdio>

struct CAFile
{
    virtual void V0() {}
    virtual void V1() {}
    virtual void V2() {}
    virtual void V3() {}
    virtual void V4() {}
    virtual void Finish(long value);
    virtual void V6() {}
    virtual long Read();
};

struct CWorldFileLoader
{
    virtual void V0() {}
    virtual void V1() {}
    virtual void V2() {}
    virtual void Load(CAFile* file, long value);
};

struct CWorld
{
    virtual void V0() {}
    virtual void FinishLoad();
    long pad[4];
    CWorldFileLoader* loader;

    void LoadWorldFromFile(CAFile* file, long value);
};

static int g_timerCalls = 0;
static int g_readCalls = 0;
static int g_loadCalls = 0;
static int g_finishCalls = 0;
static int g_worldFinishCalls = 0;
static CAFile* g_loadedFile = 0;
static long g_loadedValue = 0;
static long g_finishedValue = 0;

float CWorldLoadTimer()
{
    ++g_timerCalls;
    return 1.0f;
}

long CAFile::Read()
{
    ++g_readCalls;
    return 37;
}

void CAFile::Finish(long value)
{
    ++g_finishCalls;
    g_finishedValue = value;
}

void CWorldFileLoader::Load(CAFile* file, long value)
{
    ++g_loadCalls;
    g_loadedFile = file;
    g_loadedValue = value;
}

void CWorld::FinishLoad()
{
    ++g_worldFinishCalls;
}

void CWorld::LoadWorldFromFile(CAFile* file, long value)
{
    (void)CWorldLoadTimer();
    long result = file->Read();
    loader->Load(file, value);
    file->Finish(result);
    FinishLoad();
    (void)CWorldLoadTimer();
}

int main()
{
    CAFile file;
    CWorldFileLoader loader;
    CWorld world;
    world.loader = &loader;
    world.LoadWorldFromFile(&file, 91);
    if (g_timerCalls != 2 ||
        g_readCalls != 1 ||
        g_loadCalls != 1 ||
        g_finishCalls != 1 ||
        g_worldFinishCalls != 1 ||
        g_loadedFile != &file ||
        g_loadedValue != 91 ||
        g_finishedValue != 37)
        return 1;
    std::printf("FABLE_CWORLD_LOAD_WORLD_FROM_FILE_TEST PASS\n");
    return 0;
}