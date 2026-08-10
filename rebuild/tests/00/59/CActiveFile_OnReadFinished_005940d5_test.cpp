#include <stdio.h>
#include <stdlib.h>
struct CActiveFile { void* p0; };
static int g_base = 0;
static int g_freed = 0;
void Base_Read(CActiveFile* self){ g_base++; }
void model(CActiveFile* self)
{
    Base_Read(self);
    void* p = self->p0;
    if (p){ free(p); g_freed++; }
}
int main()
{
    CActiveFile a; a.p0 = malloc(16);
    model(&a);
    CActiveFile b; b.p0 = 0;
    model(&b);
    if (g_base==2 && g_freed==1) { printf("OK_005940d5\n"); return 0; }
    printf("FAIL\n"); return 1;
}