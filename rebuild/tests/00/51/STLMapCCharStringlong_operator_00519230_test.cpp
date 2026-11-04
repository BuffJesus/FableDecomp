// Behaviour of map<CCharString, long>::operator[](key) 0x00519230: takes the
// lower bound of the key; if it is a node with an equal key (same string data,
// or neither ordered before the other) returns that node's value; otherwise
// inserts (key, 0) with the lower bound as the hint and returns the new
// node's value (the temporary pair's key copy is destroyed).
#include "fable_stlport_tree_impl.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::map<CCharString, long> Map;
typedef Map::_Rep_type Tree;
typedef _STL::_Rb_tree_node<Map::value_type> Node;
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); }

// A string is its data pointer; the data's leading CBasicString buffer is the
// text, ordered by the real CBasicString<char>::operator< (a signed strcmp).
struct Ord { const char* text; };
CCharString::CCharString(const CCharString& o) { *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&o); Log("copy;"); }
CCharString::~CCharString() { Log("dtor;"); }
CCharString::CCharString() { *reinterpret_cast<void**>(this) = 0; }

static Node* g_lower;
static const CCharString* g_lowerKey;
template<> Tree::_Link_type Tree::_M_lower_bound(const CCharString& k) const { Log("lower;"); g_lowerKey = &k; return g_lower; }
static _STL::_Rb_tree_node_base* g_hint;
static const void* g_insertKeyData;
static long g_insertValue;
static Node g_newNode;
template<> Tree::iterator Tree::insert_unique(iterator position, const Map::value_type& v)
{
    Log("insert;");
    g_hint = position._M_node;
    g_insertKeyData = *reinterpret_cast<void* const*>(&v.first);
    g_insertValue = v.second;
    return iterator(&g_newNode);
}

static void* KeyData(const Node* n) { return *reinterpret_cast<void* const*>(&n->_M_value_field.first); }
static void SetKey(const void* key, Ord* d) { *reinterpret_cast<Ord**>(const_cast<void*>(key)) = d; }

int main()
{
    static unsigned char mapStore[sizeof(Map)];
    Map& m = *reinterpret_cast<Map*>(mapStore);
    static Node header, node;
    *reinterpret_cast<_STL::_Rb_tree_node_base**>(mapStore) = &header;
    static Ord five = { "5" }, fiveAgain = { "5" }, seven = { "7" };
    unsigned char keyStore[sizeof(CCharString)];
    CCharString& key = *reinterpret_cast<CCharString*>(keyStore);
    SetKey(&node._M_value_field.first, &five);
    node._M_value_field.second = 42;

    // Same data: found.
    SetKey(&key, &five);
    g_lower = &node;
    if (&m[key] != &node._M_value_field.second || strcmp(g_log, "lower;") != 0 || g_lowerKey != &key) { printf("same: %s\n", g_log); return 1; }
    // Equal order, other data: found.
    g_log[0] = 0;
    SetKey(&key, &fiveAgain);
    if (&m[key] != &node._M_value_field.second || strcmp(g_log, "lower;") != 0) { printf("equal: %s\n", g_log); return 1; }
    // Key ordered before the lower bound: insert (key, 0) hinted at it.
    g_log[0] = 0;
    static Ord three = { "3" };
    SetKey(&key, &three);
    if (&m[key] != &g_newNode._M_value_field.second || strcmp(g_log, "lower;copy;insert;dtor;") != 0 ||
        g_hint != &node || g_insertKeyData != &three || g_insertValue != 0) { printf("before: %s\n", g_log); return 1; }
    // Lower bound is the end: insert.
    g_log[0] = 0;
    g_lower = &header;
    SetKey(&key, &seven);
    if (&m[key] != &g_newNode._M_value_field.second || strcmp(g_log, "lower;copy;insert;dtor;") != 0 || g_hint != &header) { printf("end: %s\n", g_log); return 1; }
    // A null key sorts first: inserted before a real key.
    g_log[0] = 0;
    g_lower = &node;
    *reinterpret_cast<void**>(keyStore) = 0;
    if (&m[key] != &g_newNode._M_value_field.second || strcmp(g_log, "lower;copy;insert;dtor;") != 0) { printf("null: %s\n", g_log); return 1; }
    puts("FABLETLC_STLMAP_CCHARSTRING_LONG_INDEX_00519230_PASS");
    return 0;
}