#include <cstdio>

struct CEngineLandscapeMemoryManager {
    char pad[0x39];
    char m_poolEnabled;
    int reconfigures;
    void Reconfigure();
    void EnablePoolAllocation(bool enable);
};

void CEngineLandscapeMemoryManager::Reconfigure()
{
    reconfigures++;
}

void CEngineLandscapeMemoryManager::EnablePoolAllocation(bool enable)
{
    if (enable != (bool)m_poolEnabled) {
        Reconfigure();
        m_poolEnabled = (char)enable;
    }
}

int main()
{
    CEngineLandscapeMemoryManager m;
    m.m_poolEnabled = 0;
    m.reconfigures = 0;

    // no change: already false, set false -> no reconfigure
    m.EnablePoolAllocation(false);
    // change to true
    m.EnablePoolAllocation(true);
    if (m.m_poolEnabled != 1) { printf("FAIL1\n"); return 1; }
    if (m.reconfigures != 1) { printf("FAIL2\n"); return 1; }
    // same again -> no reconfigure
    m.EnablePoolAllocation(true);
    if (m.reconfigures != 1) { printf("FAIL3\n"); return 1; }
    // back to false
    m.EnablePoolAllocation(false);
    if (m.m_poolEnabled != 0) { printf("FAIL4\n"); return 1; }
    if (m.reconfigures != 2) { printf("FAIL5\n"); return 1; }

    printf("POOLALLOC_OK\n");
    return 0;
}