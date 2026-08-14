#include <stdio.h>
#include <stdlib.h>

static int g_allocs = 0;
static void* fake_new(unsigned int n){ g_allocs++; return malloc(n); }

static void Fill(void** begin, void** end)
{
    for (void** p = begin; p < end; ++p)
        *p = fake_new(0x70);
}

int main()
{
    const int N = 5;
    void* arr[N];
    for (int i=0;i<N;i++) arr[i]=0;
    Fill(arr, arr+N);
    int ok = (g_allocs==N);
    for (int i=0;i<N;i++){ if(!arr[i]) ok=0; }
    g_allocs=0;
    Fill(arr, arr);
    if(g_allocs!=0) ok=0;
    if(ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    for(int i=0;i<N;i++) free(arr[i]);
    return 0;
}