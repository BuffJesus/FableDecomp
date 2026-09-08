#include <stdio.h>
int main(){signed char current=3,loading=4;unsigned long frame=9;current=0;loading=0;frame=0;if(current||loading||frame)return 1;printf("FRAME_DATA_RESET_TEXTURE PASS\n");return 0;}