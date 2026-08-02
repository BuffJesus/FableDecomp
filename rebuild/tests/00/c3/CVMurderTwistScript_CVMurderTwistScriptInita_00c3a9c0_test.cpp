#include <cstdio>
#include <cstring>

class CV_MurderTwistScript
{
public:
    char pad[0x70];
    int m_field70;
    int m_field74;
    int m_field78;

    void Init();
};

void CV_MurderTwistScript::Init()
{
    m_field70 = 0;
    m_field74 = 0;
    m_field78 = 0;
}

int main()
{
    CV_MurderTwistScript obj;
    memset(&obj, 0xCD, sizeof(obj));
    obj.Init();

    if (obj.m_field70 == 0 && obj.m_field74 == 0 && obj.m_field78 == 0)
    {
        printf("INIT_OK_ZEROED_70_74_78\n");
        return 0;
    }

    printf("INIT_FAIL\n");
    return 1;
}