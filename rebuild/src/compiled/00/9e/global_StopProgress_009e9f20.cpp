struct Prog {
    virtual void m0();
    virtual void m1();
    virtual void stop(bool a, bool b);
};
Prog* g_prog;

void __fastcall StopProgress(bool a, bool b)
{
    Prog* p = g_prog;
    if (p)
        p->stop(a, b);
}