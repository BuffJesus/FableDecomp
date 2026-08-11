#include <stdio.h>

// Independent behaviour model of CTCStockItem::OnInitialActivate.
// Logic: char flag @ +0x0c. If Check()==0 OR flag==0, force flag=0;
// else (Check()!=0 AND flag!=0) leave flag untouched.

struct Stock {
    char pad[0xc];
    char flag;
};

static void OnInitialActivate_model(Stock* self, int checkResult)
{
    if (checkResult == 0 || self->flag == 0)
        self->flag = 0;
}

int main()
{
    int ok = 1;

    // Check!=0, flag!=0 -> unchanged
    Stock a; a.flag = 5;
    OnInitialActivate_model(&a, 1);
    if (a.flag != 5) ok = 0;

    // Check!=0, flag==0 -> stays 0
    Stock b; b.flag = 0;
    OnInitialActivate_model(&b, 1);
    if (b.flag != 0) ok = 0;

    // Check==0, flag!=0 -> forced to 0
    Stock c; c.flag = 7;
    OnInitialActivate_model(&c, 0);
    if (c.flag != 0) ok = 0;

    // Check==0, flag==0 -> stays 0
    Stock d; d.flag = 0;
    OnInitialActivate_model(&d, 0);
    if (d.flag != 0) ok = 0;

    if (ok) { printf("STOCK_OIA_OK\n"); return 0; }
    printf("STOCK_OIA_FAIL\n"); return 1;
}