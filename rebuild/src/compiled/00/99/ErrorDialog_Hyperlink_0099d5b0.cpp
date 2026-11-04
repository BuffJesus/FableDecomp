// bbblibrary/errordialog.cpp hyperlink statics (FableWin names; Microsoft's
// "convert a static control to a hyperlink" sample in its ANSI form, with
// window properties "Old_Proc", "Old_Font", "Font" and "Static"):
// _HyperlinkParentProc 0099D5B0, _HyperlinkProc 0099D660 and
// ConvertStaticToHyperlink 0099D7D0. Lionhead's version keeps the hover state
// in one global (red while hovered, else blue) and also sends WM_SETFONT with
// the underlined font. user32/gdi32 are called as dllimports; the handle and
// structure types are declared as <windows.h> declares them.

struct HWND__;
struct HDC__;
struct HFONT__;
struct HICON__;
struct HINSTANCE__;
typedef long (__stdcall *WindowProcedure)(HWND__*, unsigned int, unsigned int, long);

struct tagRECT { long left, top, right, bottom; };
struct tagPOINT { long x, y; };
struct tagLOGFONTA
{
    long lfHeight, lfWidth, lfEscapement, lfOrientation, lfWeight;
    unsigned char lfItalic, lfUnderline, lfStrikeOut, lfCharSet;
    unsigned char lfOutPrecision, lfClipPrecision, lfQuality, lfPitchAndFamily;
    char lfFaceName[32];
};

extern "C" __declspec(dllimport) void* __stdcall GetPropA(HWND__* window, const char* name);
extern "C" __declspec(dllimport) int __stdcall SetPropA(HWND__* window, const char* name, void* data);
extern "C" __declspec(dllimport) void* __stdcall RemovePropA(HWND__* window, const char* name);
extern "C" __declspec(dllimport) long __stdcall GetWindowLongA(HWND__* window, int index);
extern "C" __declspec(dllimport) long __stdcall SetWindowLongA(HWND__* window, int index, long value);
extern "C" __declspec(dllimport) long __stdcall CallWindowProcA(WindowProcedure procedure, HWND__* window,
                                                               unsigned int message, unsigned int wParam, long lParam);
extern "C" __declspec(dllimport) long __stdcall SendMessageA(HWND__* window, unsigned int message,
                                                            unsigned int wParam, long lParam);
extern "C" __declspec(dllimport) HWND__* __stdcall GetParent(HWND__* window);
extern "C" __declspec(dllimport) HWND__* __stdcall GetCapture();
extern "C" __declspec(dllimport) HWND__* __stdcall SetCapture(HWND__* window);
extern "C" __declspec(dllimport) int __stdcall ReleaseCapture();
extern "C" __declspec(dllimport) int __stdcall InvalidateRect(HWND__* window, const tagRECT* rect, int erase);
extern "C" __declspec(dllimport) int __stdcall GetWindowRect(HWND__* window, tagRECT* rect);
extern "C" __declspec(dllimport) int __stdcall ClientToScreen(HWND__* window, tagPOINT* point);
extern "C" __declspec(dllimport) int __stdcall PtInRect(const tagRECT* rect, tagPOINT point);
extern "C" __declspec(dllimport) HICON__* __stdcall LoadCursorA(HINSTANCE__* instance, const char* name);
extern "C" __declspec(dllimport) HICON__* __stdcall SetCursor(HICON__* cursor);
extern "C" __declspec(dllimport) unsigned long __stdcall SetTextColor(HDC__* dc, unsigned long colour);
extern "C" __declspec(dllimport) int __stdcall GetObjectA(void* object, int size, void* buffer);
extern "C" __declspec(dllimport) HFONT__* __stdcall CreateFontIndirectA(const tagLOGFONTA* font);
extern "C" __declspec(dllimport) int __stdcall DeleteObject(void* object);

#define PROP_ORIGINAL_PROC "Old_Proc"
#define PROP_ORIGINAL_FONT "Old_Font"
#define PROP_UNDERLINE_FONT "Font"
#define PROP_STATIC_HYPERLINK "Static"

// Ego_r errordialog.cpp global (retail 0x013BD6F8): the hyperlink under the
// mouse is drawn highlighted.
extern int MouseOver;

long __stdcall _HyperlinkParentProc(HWND__* hwnd, unsigned int message, unsigned int wParam, long lParam)
{
    WindowProcedure pfnOrigProc = reinterpret_cast<WindowProcedure>(GetPropA(hwnd, PROP_ORIGINAL_PROC));
    switch (message)
    {
    case 0x138: // WM_CTLCOLORSTATIC
        {
            HDC__* hdc = reinterpret_cast<HDC__*>(wParam);
            HWND__* hwndCtl = reinterpret_cast<HWND__*>(lParam);
            if (GetPropA(hwndCtl, PROP_STATIC_HYPERLINK) != 0)
            {
                long lr = CallWindowProcA(pfnOrigProc, hwnd, message, wParam, lParam);
                if (MouseOver)
                    SetTextColor(hdc, 0x0000E0); // RGB(224, 0, 0)
                else
                    SetTextColor(hdc, 0xC00000); // RGB(0, 0, 192)
                return lr;
            }
            break;
        }
    case 2: // WM_DESTROY
        SetWindowLongA(hwnd, -4, reinterpret_cast<long>(pfnOrigProc));
        RemovePropA(hwnd, PROP_ORIGINAL_PROC);
        break;
    }
    return CallWindowProcA(pfnOrigProc, hwnd, message, wParam, lParam);
}

long __stdcall _HyperlinkProc(HWND__* hwnd, unsigned int message, unsigned int wParam, long lParam)
{
    WindowProcedure pfnOrigProc = reinterpret_cast<WindowProcedure>(GetPropA(hwnd, PROP_ORIGINAL_PROC));
    switch (message)
    {
    case 2: // WM_DESTROY
        {
            SetWindowLongA(hwnd, -4, reinterpret_cast<long>(pfnOrigProc));
            RemovePropA(hwnd, PROP_ORIGINAL_PROC);
            HFONT__* hOrigFont = static_cast<HFONT__*>(GetPropA(hwnd, PROP_ORIGINAL_FONT));
            SendMessageA(hwnd, 0x30, reinterpret_cast<unsigned int>(hOrigFont), 0); // WM_SETFONT
            RemovePropA(hwnd, PROP_ORIGINAL_FONT);
            HFONT__* hFont = static_cast<HFONT__*>(GetPropA(hwnd, PROP_UNDERLINE_FONT));
            DeleteObject(hFont);
            RemovePropA(hwnd, PROP_UNDERLINE_FONT);
            RemovePropA(hwnd, PROP_STATIC_HYPERLINK);
            break;
        }
    case 0x200: // WM_MOUSEMOVE
        if (GetCapture() != hwnd)
        {
            MouseOver = 1;
            InvalidateRect(hwnd, 0, 0);
            SetCapture(hwnd);
        }
        else
        {
            tagRECT rect;
            GetWindowRect(hwnd, &rect);
            // LOWORD / HIWORD of the client position.
            tagPOINT pt = { static_cast<unsigned short>(static_cast<unsigned long>(lParam) & 0xFFFF),
                            static_cast<unsigned short>((static_cast<unsigned long>(lParam) >> 16) & 0xFFFF) };
            ClientToScreen(hwnd, &pt);
            if (!PtInRect(&rect, pt))
            {
                MouseOver = 0;
                InvalidateRect(hwnd, 0, 0);
                ReleaseCapture();
            }
        }
        break;
    case 0x20: // WM_SETCURSOR
        {
            HICON__* hCursor = LoadCursorA(0, reinterpret_cast<const char*>(32649)); // IDC_HAND
            if (hCursor == 0)
                hCursor = LoadCursorA(0, reinterpret_cast<const char*>(32512)); // IDC_ARROW
            SetCursor(hCursor);
            return 1;
        }
    }
    return CallWindowProcA(pfnOrigProc, hwnd, message, wParam, lParam);
}

int __fastcall ConvertStaticToHyperlink(HWND__* hwndCtl)
{
    // Subclass the parent so it can colour the control.
    HWND__* hwndParent = GetParent(hwndCtl);
    if (hwndParent != 0)
    {
        WindowProcedure pfnOrigProc = reinterpret_cast<WindowProcedure>(GetWindowLongA(hwndParent, -4));
        if (pfnOrigProc != _HyperlinkParentProc)
        {
            SetPropA(hwndParent, PROP_ORIGINAL_PROC, reinterpret_cast<void*>(pfnOrigProc));
            SetWindowLongA(hwndParent, -4, reinterpret_cast<long>(_HyperlinkParentProc));
        }
    }

    // The control must send notifications (SS_NOTIFY).
    long dwStyle = GetWindowLongA(hwndCtl, -16);
    SetWindowLongA(hwndCtl, -16, dwStyle | 0x100);

    // Subclass the control itself.
    WindowProcedure pfnOrigProc = reinterpret_cast<WindowProcedure>(GetWindowLongA(hwndCtl, -4));
    SetPropA(hwndCtl, PROP_ORIGINAL_PROC, reinterpret_cast<void*>(pfnOrigProc));
    SetWindowLongA(hwndCtl, -4, reinterpret_cast<long>(_HyperlinkProc));

    // An underlined copy of its font.
    HFONT__* hOrigFont = reinterpret_cast<HFONT__*>(SendMessageA(hwndCtl, 0x31, 0, 0)); // WM_GETFONT
    SetPropA(hwndCtl, PROP_ORIGINAL_FONT, hOrigFont);
    tagLOGFONTA lf;
    GetObjectA(hOrigFont, sizeof(lf), &lf);
    lf.lfUnderline = 1;
    HFONT__* hFont = CreateFontIndirectA(&lf);
    SetPropA(hwndCtl, PROP_UNDERLINE_FONT, hFont);
    SendMessageA(hwndCtl, 0x30, reinterpret_cast<unsigned int>(hFont), 0); // WM_SETFONT

    SetPropA(hwndCtl, PROP_STATIC_HYPERLINK, reinterpret_cast<void*>(1));
    return 1;
}
