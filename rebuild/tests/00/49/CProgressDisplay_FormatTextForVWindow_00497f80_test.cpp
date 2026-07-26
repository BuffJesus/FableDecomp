#include <stdio.h>

#include "fable_string.h"

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

class CFontBank
{
};

class CProgressDisplayFormatTextView
{
public:
    CWideString WordWrapText(
        const C2DBoxF& window,
        const CFontBank& fontBank,
        const CWideString& text) const;

    bool FormatTextForVWindow(
        const C2DBoxF& window,
        const CFontBank& fontBank,
        const CWideString& text,
        CWideString& formattedText,
        C2DBoxF& formattedWindow) const;
};

static CWideStringData g_DefaultStorage = {0, 0, 0, 1};
static CWideStringData g_WrappedStorage = {0, 0, 0, 1};
static CWideStringData* g_NextStorage = &g_DefaultStorage;
static int g_WrapCalls = 0;

CWideString::CWideString()
    : storage_(g_NextStorage)
{
}

CWideString::CWideString(const CWideString& other)
    : storage_(*reinterpret_cast<CWideStringData* const*>(&other))
{
}

CWideString::~CWideString()
{
}

CWideString& CWideString::operator=(const CWideString& other)
{
    storage_ = *reinterpret_cast<CWideStringData* const*>(&other);
    return *this;
}

CWideString CProgressDisplayFormatTextView::WordWrapText(
    const C2DBoxF&,
    const CFontBank&,
    const CWideString&) const
{
    ++g_WrapCalls;
    g_NextStorage = &g_WrappedStorage;
    CWideString result;
    g_NextStorage = &g_DefaultStorage;
    return result;
}

int main()
{
    CProgressDisplayFormatTextView display;
    CFontBank font;
    CWideString input;
    CWideString output;
    const C2DBoxF inputWindow = {10.0f, 20.0f, 310.0f, 220.0f};
    C2DBoxF outputWindow = {0.0f, 0.0f, 0.0f, 0.0f};

    const bool changed = display.FormatTextForVWindow(
        inputWindow,
        font,
        input,
        output,
        outputWindow);
    CWideStringData* outputStorage =
        *reinterpret_cast<CWideStringData**>(&output);

    if (
        changed ||
        g_WrapCalls != 1 ||
        outputStorage != &g_WrappedStorage ||
        outputWindow.left != inputWindow.left ||
        outputWindow.top != inputWindow.top ||
        outputWindow.right != inputWindow.right ||
        outputWindow.bottom != inputWindow.bottom)
    {
        return 1;
    }

    printf("FABLETLC_FORMAT_TEXT_FOR_VWINDOW_BEHAVIOR PASS\n");
    return 0;
}
