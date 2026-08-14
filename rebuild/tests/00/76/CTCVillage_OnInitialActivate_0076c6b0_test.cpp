#include <cstdio>

struct Inner;

static int g_called = 0;
static void* g_ecx = 0;
static void* g_edx = 0;
static unsigned g_arg = 0;

void __fastcall Inner_Do(Inner* self, void* edx, unsigned char arg) {
    g_called++; g_ecx = self; g_edx = edx; g_arg = arg;
}

struct CTCVillage { Inner* p0; void* p4; char pad8[4]; unsigned char c; unsigned char d; };

void __fastcall OnInitialActivate(CTCVillage* self) {
    if (self->d) { Inner_Do(self->p0, self->p4, self->c); self->d = 0; }
}

int main() {
    CTCVillage v; v.p0=(Inner*)0x1111; v.p4=(void*)0x2222; v.c=0x5A; v.d=0;
    OnInitialActivate(&v); if (g_called!=0){printf("FAIL1\n");return 1;}
    v.d=1; OnInitialActivate(&v);
    if (g_called!=1){printf("FAIL2\n");return 1;}
    if (g_ecx!=(void*)0x1111){printf("FAIL3\n");return 1;}
    if (g_edx!=(void*)0x2222){printf("FAIL4\n");return 1;}
    if (g_arg!=0x5A){printf("FAIL5\n");return 1;}
    if (v.d!=0){printf("FAIL6\n");return 1;}
    OnInitialActivate(&v); if (g_called!=1){printf("FAIL7\n");return 1;}
    printf("PARITY_OK\n"); return 0;
}