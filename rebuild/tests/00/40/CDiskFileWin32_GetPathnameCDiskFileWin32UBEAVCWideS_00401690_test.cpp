#include <cstdio>
#include <cstring>

struct CWideString {
    char* p;
    CWideString(const char* s) { p = (char*)s; }
};

struct CDiskFileWin32 {
    char pad[0x18];
    char name[4];
    CWideString GetPathname() const;
};

CWideString CDiskFileWin32::GetPathname() const {
    return CWideString(this->name);
}

int main() {
    CDiskFileWin32 f;
    std::memset(&f, 0, sizeof(f));
    f.name[0] = 'A'; f.name[1] = 'B'; f.name[2] = 0;
    CWideString r = f.GetPathname();
    if (r.p == f.name && r.p[0] == 'A' && r.p[1] == 'B') {
        std::printf("CDiskFileWin32_00401690_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}