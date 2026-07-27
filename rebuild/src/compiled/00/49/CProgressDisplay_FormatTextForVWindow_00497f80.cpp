#include "fable_string.h"

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

class CFontBank;

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

bool CProgressDisplayFormatTextView::FormatTextForVWindow(
    const C2DBoxF& window,
    const CFontBank& fontBank,
    const CWideString& text,
    CWideString& formattedText,
    C2DBoxF& formattedWindow) const
{
    formattedText = WordWrapText(window, fontBank, text);
    formattedWindow = window;
    return false;
}
