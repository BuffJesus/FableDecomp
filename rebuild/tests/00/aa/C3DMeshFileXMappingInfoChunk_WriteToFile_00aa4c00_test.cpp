#include <stdio.h>
struct Stream{int Count;char Values[2];Stream():Count(0){}void WriteCBYTE(char v){Values[Count++]=v;}};
int main(){Stream s;s.WriteCBYTE(1);s.WriteCBYTE((char)7);if(s.Count!=2||s.Values[0]!=1||s.Values[1]!=7)return 1;printf("MESH_MAPPING_CHUNK_WRITE PASS\n");return 0;}