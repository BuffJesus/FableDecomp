# Retired allocator frame mislabel

The old source exposed a free fastcall ClearPerformanceStats using invented globals instead of the native CMemoryAllocator::NewFrame member and shared critical section. Its test printed PASS without calling any provider. Retail 0x009F26D0 and donor/header evidence identify NewFrame. The replacement uses the shared class/imports, matches all 23 retail bytes, and runs three state/lock cases with a provider omission check.
