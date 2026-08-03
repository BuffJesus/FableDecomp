class CCharString {
public:
    char *m_data;
    CCharString();
};

extern "C" long g_charStringCount;
long g_charStringCount = 0;

CCharString::CCharString()
{
    m_data = 0;
    ++g_charStringCount;
}