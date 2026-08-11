#include <stdio.h>

struct CBase {
    int a;
    int b;
    int c;
    CBase& operator=(const CBase& rhs);
};

CBase& CBase::operator=(const CBase& rhs) {
    a = rhs.a;
    b = rhs.b;
    c = rhs.c;
    return *this;
}

struct CTavernTableDef : public CBase {
    char m_flag;
    CTavernTableDef& operator=(const CTavernTableDef& rhs);
};

CTavernTableDef& CTavernTableDef::operator=(const CTavernTableDef& rhs) {
    CBase::operator=(rhs);
    m_flag = rhs.m_flag;
    return *this;
}

int main() {
    CTavernTableDef src;
    src.a = 11; src.b = 22; src.c = 33; src.m_flag = (char)0x7E;

    CTavernTableDef dst;
    dst.a = 0; dst.b = 0; dst.c = 0; dst.m_flag = 0;

    CTavernTableDef* ret = &(dst = src);

    bool ok = (ret == &dst)
           && dst.a == 11 && dst.b == 22 && dst.c == 33
           && dst.m_flag == (char)0x7E;

    if (ok) {
        printf("TAVERN_ASSIGN_OK\n");
        return 0;
    }
    printf("TAVERN_ASSIGN_FAIL\n");
    return 1;
}