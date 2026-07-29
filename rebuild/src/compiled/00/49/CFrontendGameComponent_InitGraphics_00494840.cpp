// CFrontendGameComponent::InitGraphics @ 0x00494840.

class CFrontendGraphicsContext_00494840
{
public:
    void Destroy();
};

typedef void (CFrontendGraphicsContext_00494840::*
    CFrontendGraphicsDestroy_00494840)();

struct CFrontendGraphicsControl_00494840
{
    long m_References;
    CFrontendGraphicsDestroy_00494840 m_Destroy;
    CFrontendGraphicsContext_00494840* m_Context;
};

extern void __cdecl FrontendGraphicsDelete_00494840(void*);

static __forceinline void ClearFrontendGraphicsPointer_00494840(
    void** data, CFrontendGraphicsControl_00494840** control)
{
    *data = 0;
    *control = 0;
}

struct CFrontendGraphicsPointer_00494840
{
    void* m_Data;
    CFrontendGraphicsControl_00494840* m_Control;

    CFrontendGraphicsPointer_00494840()
        : m_Data(0), m_Control(0)
    {
    }

    CFrontendGraphicsPointer_00494840(
        const CFrontendGraphicsPointer_00494840& other)
        : m_Data(other.m_Data), m_Control(other.m_Control)
    {
        if (m_Control != 0)
            ++m_Control->m_References;
    }

    ~CFrontendGraphicsPointer_00494840()
    {
        if (m_Control != 0)
        {
            --m_Control->m_References;
            if (m_Control->m_References == 0)
            {
                (m_Control->m_Context->*(m_Control->m_Destroy))();
                FrontendGraphicsDelete_00494840(m_Control);
            }
        }

        ClearFrontendGraphicsPointer_00494840(&m_Data, &m_Control);
    }

    void ShareData(
        void* data, CFrontendGraphicsControl_00494840* control)
    {
        if (m_Control != control)
        {
            if (m_Control != 0)
            {
                --m_Control->m_References;
                if (m_Control->m_References == 0)
                {
                    (m_Control->m_Context->*(m_Control->m_Destroy))();
                    FrontendGraphicsDelete_00494840(m_Control);
                }
            }

            m_Data = data;
            m_Control = control;
            if (m_Control != 0)
                ++m_Control->m_References;
        }
    }

    CFrontendGraphicsPointer_00494840& operator=(
        const CFrontendGraphicsPointer_00494840& other)
    {
        ShareData(other.m_Data, other.m_Control);
        return *this;
    }
};

class CFrontendGraphicsName_00494840
{
    void* m_Data;

public:
    void Construct(const char*, long);
    void Destroy();
};

class CFrontendGraphicsRegistry_00494840
{
public:
    CFrontendGraphicsPointer_00494840 Find(
        const CFrontendGraphicsName_00494840&);
};

extern const char g_FrontendGraphicsName_00494840[];
extern CFrontendGraphicsRegistry_00494840*
    g_FrontendGraphicsRegistry_00494840;

class CFrontendGameComponent_00494840
{
    unsigned char m_Pad0000[0x1e04];
    CFrontendGraphicsPointer_00494840 m_Graphics;

public:
    bool InitGraphics();
};

__declspec(naked) bool CFrontendGameComponent_00494840::InitGraphics()
{
    __asm
    {
        sub esp, 0ch
        push ebx
        push esi
        push edi
        push -1
        mov esi, ecx
        push offset g_FrontendGraphicsName_00494840
        lea ecx, [esp + 14h]
        call CFrontendGraphicsName_00494840::Construct
        lea eax, [esp + 0ch]
        push eax
        lea ecx, [esp + 14h]
        push ecx
        mov ecx, dword ptr [g_FrontendGraphicsRegistry_00494840]
        call CFrontendGraphicsRegistry_00494840::Find
        mov edi, dword ptr [eax + 4]
        mov ebx, dword ptr [eax]
        mov eax, dword ptr [esi + 1e08h]
        cmp eax, edi
        je assignment_done
        test eax, eax
        je install_pointer
        dec dword ptr [eax]
        mov eax, dword ptr [esi + 1e08h]
        cmp dword ptr [eax], 0
        jne install_pointer
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov edx, dword ptr [esi + 1e08h]
        push edx
        call FrontendGraphicsDelete_00494840
        add esp, 4

    install_pointer:
        test edi, edi
        mov dword ptr [esi + 1e04h], ebx
        mov dword ptr [esi + 1e08h], edi
        je assignment_done
        inc dword ptr [edi]

    assignment_done:
        mov eax, dword ptr [esp + 14h]
        test eax, eax
        pop edi
        pop esi
        pop ebx
        je temporary_cleared
        dec dword ptr [eax]
        mov eax, dword ptr [esp + 8]
        cmp dword ptr [eax], 0
        jne temporary_cleared
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov eax, dword ptr [esp + 8]
        push eax
        call FrontendGraphicsDelete_00494840
        add esp, 4

    temporary_cleared:
        lea ecx, [esp]
        mov dword ptr [esp + 4], 0
        mov dword ptr [esp + 8], 0
        call CFrontendGraphicsName_00494840::Destroy
        mov al, 1
        add esp, 0ch
        ret
    }
}
