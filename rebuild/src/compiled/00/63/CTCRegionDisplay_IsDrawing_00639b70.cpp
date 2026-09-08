#include <wchar.h>
struct CWideString {
    void* Data;
    bool operator!=(const wchar_t* value) const;
};
extern const wchar_t EmptyWideString[];
struct CTCRegionDisplay {
    unsigned char Base[0x0c];
    CWideString CurrentAreaName;
    CWideString CurrentAreaNameExtra;
    long FrameNameChanged;
    long CurrentAlpha;
    long LastAlpha;
    float DisplayTime;
    bool TimeOut;
    bool InfoDisplay;
    bool UpdateWhilePaused;
    bool IsDrawing() const;
};
bool CTCRegionDisplay::IsDrawing() const {
    return CurrentAlpha > 0 && CurrentAreaName != EmptyWideString;
}