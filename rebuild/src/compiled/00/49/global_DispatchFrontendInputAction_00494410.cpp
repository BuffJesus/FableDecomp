// Uncataloged frontend input-action coordinator @ 0x00494410.

class CResolvedFrontendInputAction_00494410
{
public:
    void Dispatch();
};

extern CResolvedFrontendInputAction_00494410* __stdcall
ResolveFrontendInputAction_00494410(void*, void*);

void __stdcall DispatchFrontendInputAction_00494410(
    void* first, void* second)
{
    ResolveFrontendInputAction_00494410(first, second)->Dispatch();
}
