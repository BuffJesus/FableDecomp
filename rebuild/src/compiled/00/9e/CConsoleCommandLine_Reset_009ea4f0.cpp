struct CCharString {
    int Data;
    CCharString& operator=(const char* text);
};

struct CConsoleCommandLine {
    unsigned char _base_0x00[4];
    CCharString CurrentEntry;
    bool UserEntry;
    unsigned char _pad_0x09[3];
    long CursorPosition;

    void Reset();
};

void CConsoleCommandLine::Reset() {
    CurrentEntry = "";
    UserEntry = false;
    CursorPosition = 0;
}