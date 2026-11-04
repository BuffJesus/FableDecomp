#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_checksum_cache_file.h"
static CChecksumCacheFile* cache;
static char events[16]; static unsigned count;
static void record(char c) { events[count++] = c; events[count] = 0; }
static void require(bool condition) { if (!condition) { printf("failed after %s\n", events); exit(1); } }
void CChecksumCacheFile::Close() {
    require(this == cache && cache->BufferSize == 0x10000 && cache->NeedsSaving);
    record('S');
}
CWideString::~CWideString() { require(this == &cache->Filename); record('W'); }
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::_Rep_type CacheTree;
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type CacheValue;
typedef _STL::_Rb_tree_node<CacheValue> CacheNode;
template<> void CacheTree::_M_erase(CacheNode* node) {
    require(this == &cache->Entries._M_t && node != 0 && strcmp(events, "S") == 0);
    record('E'); free(node);
}
int main() {
    for (unsigned mode = 0; mode < 2; ++mode) {
        unsigned char raw[sizeof(CChecksumCacheFile)]; memset(raw, 0, sizeof(raw));
        cache = reinterpret_cast<CChecksumCacheFile*>(raw);
        cache->BufferSize = 0x10000; cache->NeedsSaving = true;
        CacheNode* header = static_cast<CacheNode*>(malloc(sizeof(CacheNode)));
        memset(header, 0, sizeof(*header));
        cache->Entries._M_t._M_header._M_data = header;
        if (mode) {
            header->_M_parent = static_cast<CacheNode*>(malloc(sizeof(CacheNode)));
            cache->Entries._M_t._M_node_count = 1;
        }
        count = 0;
        cache->~CChecksumCacheFile();
        require(strcmp(events, mode ? "SEW" : "SW") == 0);
    }
    puts("CHECKSUM_CACHE_DTOR_00a60ce0_PASS");
    return 0;
}
