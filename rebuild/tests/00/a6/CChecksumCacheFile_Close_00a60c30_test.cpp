#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_checksum_cache_file.h"
static CChecksumCacheFile* cache;
static long length;
static char events[16]; static unsigned count;
static void record(char c) { events[count++] = c; events[count] = 0; }
static void require(bool condition) { if (!condition) { printf("failed after %s\n", events); exit(1); } }
long CWideString::GetLength() const { require(this == &cache->Filename); record('L'); return length; }
void CChecksumCacheFile::Save() { require(this == cache); record('S'); }
const CWideString& CWideString::operator=(const wchar_t* value) {
    require(this == &cache->Filename && value && !*value && cache->Entries._M_t._M_node_count == 0);
    record('A'); length = 0; return *this;
}
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::_Rep_type CacheTree;
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type CacheValue;
typedef _STL::_Rb_tree_node<CacheValue> CacheNode;
template<> void CacheTree::_M_erase(CacheNode* node) {
    require(this == &cache->Entries._M_t && node == reinterpret_cast<CacheNode*>(1));
    record('E');
}
int main() {
    for (unsigned mode = 0; mode < 3; ++mode) {
        unsigned char raw[sizeof(CChecksumCacheFile)]; memset(raw, 0, sizeof(raw));
        cache = reinterpret_cast<CChecksumCacheFile*>(raw);
        _STL::_Rb_tree_node_base header; memset(&header, 0, sizeof(header));
        header._M_parent = reinterpret_cast<CacheNode*>(1);
        cache->Entries._M_t._M_header._M_data = reinterpret_cast<CacheNode*>(&header);
        cache->Entries._M_t._M_node_count = mode == 2 ? 0 : 1;
        length = mode == 0 ? 0 : 5;
        count = 0;
        cache->Close();
        require(strcmp(events, mode == 0 ? "L" : mode == 1 ? "LSEA" : "LSA") == 0);
        if (mode == 0) require(cache->Entries._M_t._M_node_count == 1 && header._M_parent == reinterpret_cast<CacheNode*>(1));
        if (mode == 1) require(header._M_parent == 0 && header._M_left == &header && header._M_right == &header);
        count = 0;
        cache->Close();
        require(strcmp(events, "L") == 0);
    }
    puts("CHECKSUM_CACHE_CLOSE_00a60c30_PASS");
    return 0;
}
