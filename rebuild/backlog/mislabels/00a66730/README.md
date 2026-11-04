# Retired allocator virtual-memory alias

Donor PDB/DIA assigns this function to the fastcall class-specific CMemoryAllocatorBase allocation/release operator. The old source used an invented global function-pointer boundary and its test exercised a local imitation. The replacement uses the shared class and actual Win32 imports; both operators match all 32 retail bytes and are exercised together with real virtual-memory pages and provider omission checks.
