#include "fable_date_and_time.h"
#include <stdio.h>
int main() {
    CDateAndTime value(-123,0xFFFFFFFF,0x80000000,23,59,0xFFFF,999);
    if(sizeof(value)!=28 || value.Year!=-123 || value.Month!=0xFFFFFFFF || value.Day!=0x80000000 || value.Hour!=23 || value.Minute!=59 || value.Second!=0xFFFF || value.Millisecond!=999)return 1;
    puts("DATE_CONSTRUCTOR_009e9a70_PASS");return 0;
}
