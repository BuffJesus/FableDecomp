#include "rebuild_abi.h"
#include <cstdio>
struct Entry72 { char raw[0x48]; };
struct CEditWorldMap { char pad[0x20]; Entry72 *m_arr; void Set(int idx, char v); };
void CEditWorldMap::Set(int idx, char v){ *((char*)&m_arr[idx]+0x27)=v; }
int main(){ CEditWorldMap o; Entry72 arr[4]; o.m_arr=arr; o.Set(2,'Q');
 if(*((char*)&arr[2]+0x27)!='Q'){std::printf("B1\n");return 1;}
 std::printf("MC_4fcfe0_OK\n"); return 0; }