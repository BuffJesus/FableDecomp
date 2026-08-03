struct CWorld {
  char pad[0x10];
  char* m_map; // +0x10
  void* DrawGetWorldMap();
};
void* CWorld::DrawGetWorldMap() {
  char* p = m_map;      // mov eax,[ecx+0x10]
  if (p != 0)           // test eax,eax / je
    return p + 4;       // add eax,4 / ret
  return 0;             // xor eax,eax / ret
}