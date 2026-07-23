struct Obj {
    int a;
    int b;
    void Add(const char* name, int flag);
    void Set(const char* name, int flag);
    void Apply();
};

extern const char g_nameA[];
extern const char g_nameB[];

void GFClearBadExitFlag()
{
    Obj o;
    o.Add(g_nameA, 0);
    o.Set(g_nameB, 0);
    o.Apply();
}