#include <stdlib.h>
#include <stdio.h>

struct CActiveFile
{
    void* m_buffer;
    void Cleanup();
    void OnReadFinished();
};

static int g_cleanupCalls = 0;

void CActiveFile::Cleanup()
{
    g_cleanupCalls++;
}

void CActiveFile::OnReadFinished()
{
    this->Cleanup();
    void* p = this->m_buffer;
    if (p)
    {
        free(p);
    }
}

int main()
{
    CActiveFile a;
    a.m_buffer = malloc(64);
    a.OnReadFinished();

    CActiveFile b;
    b.m_buffer = 0;
    b.OnReadFinished();

    if (g_cleanupCalls == 2)
    {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}