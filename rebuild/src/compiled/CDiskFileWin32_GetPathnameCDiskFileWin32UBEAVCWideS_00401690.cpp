struct CWideString {
    char* p;
    CWideString(const char* s);
};

struct CDiskFileWin32 {
    char pad[0x18];
    char name[4];
    CWideString GetPathname() const;
};

CWideString CDiskFileWin32::GetPathname() const {
    return CWideString(this->name);
}