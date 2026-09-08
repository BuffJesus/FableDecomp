#include <stdio.h>
struct Device { unsigned long LastFormat; void SetFVF(unsigned long value){LastFormat=value;} };
struct Manager { Device* PD3DDevice; void SetVertexFormat(unsigned long); };
void Manager::SetVertexFormat(unsigned long value){PD3DDevice->SetFVF(value);}
int main(){Device device;device.LastFormat=0;Manager manager;manager.PD3DDevice=&device;
 manager.SetVertexFormat(0x11223344);if(device.LastFormat!=0x11223344)return 1;
 printf("RENDER_MANAGER_SET_VERTEX_FORMAT PASS\n");return 0;}