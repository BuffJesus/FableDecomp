#include "fable_stlport_tree.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<const CCharString, long> Value;
typedef _STL::_Rb_tree<CCharString, Value, _STL::_Select1st<Value>, _STL::less<CCharString>,
                       _STL::allocator<Value> > Tree;
typedef _STL::_Rb_tree_node<Value> Node;

// A string is its data pointer; the data's first field (CBasicString's
// PData) is the text, ordered by the real CBasicString<char>::Compare.
struct Ord { const char* text; };

// The static nodes' keys start null.
CCharString::CCharString() { *reinterpret_cast<void**>(this) = 0; }
CCharString::~CCharString() {}

static void SetKey(const CCharString* key, Ord* d) { *reinterpret_cast<Ord**>(const_cast<CCharString*>(key)) = d; }
static void Link(Node* parent, Node* left, Node* right)
{
    parent->_M_left = left; parent->_M_right = right;
    if (left) left->_M_parent = parent;
    if (right) right->_M_parent = parent;
}

int main()
{
    static unsigned char treeStore[sizeof(Tree)];
    Tree& t = *reinterpret_cast<Tree*>(treeStore);
    static Node header, n3, n5, n7;
    *reinterpret_cast<_STL::_Rb_tree_node_base**>(treeStore) = &header;
    unsigned char keyStore[sizeof(CCharString)];
    const CCharString& key = *reinterpret_cast<CCharString*>(keyStore);
    static Ord three = { "3" }, five = { "5" }, fiveAgain = { "5" }, seven = { "7" }, four = { "4" }, one = { "1" }, nine = { "9" };

    // Empty tree: end().
    SetKey(&key, &five);
    if (t._M_lower_bound(key) != &header) { puts("empty"); return 1; }

    // 5 at the root, 3 left, 7 right.
    header._M_parent = &n5; n5._M_parent = &header;
    Link(&n5, &n3, &n7); Link(&n3, 0, 0); Link(&n7, 0, 0);
    SetKey(&n3._M_value_field.first, &three);
    SetKey(&n5._M_value_field.first, &five);
    SetKey(&n7._M_value_field.first, &seven);

    SetKey(&key, &five);        // same data: found
    if (t._M_lower_bound(key) != &n5) { puts("same data"); return 1; }
    SetKey(&key, &fiveAgain);   // equal text, other data: found
    if (t._M_lower_bound(key) != &n5) { puts("equal text"); return 1; }
    SetKey(&key, &seven);
    if (t._M_lower_bound(key) != &n7) { puts("leaf right"); return 1; }
    SetKey(&key, &three);
    if (t._M_lower_bound(key) != &n3) { puts("leaf left"); return 1; }
    SetKey(&key, &four);        // between 3 and 5: lower bound 5, not equal
    if (t._M_lower_bound(key) != &n5) { puts("absent between"); return 1; }
    SetKey(&key, &one);
    if (t._M_lower_bound(key) != &n3) { puts("absent low"); return 1; }
    SetKey(&key, &nine);        // above all: the candidate stays the header
    if (t._M_lower_bound(key) != &header) { puts("absent high"); return 1; }
    SetKey(&key, 0);            // a null string sorts first: absent
    if (t._M_lower_bound(key) != &n3) { puts("null key"); return 1; }
    SetKey(&n3._M_value_field.first, 0);   // ...and finds a null-keyed node
    if (t._M_lower_bound(key) != &n3) { puts("null node"); return 1; }

    // Signed-char comparison: a high-bit character precedes ASCII, unlike
    // unsigned strcmp ordering. Use a single-node tree for this edge case.
    static Ord negative = { "\x80" }, ascii = { "0" };
    header._M_parent=&n5; Link(&n5,0,0); SetKey(&n5._M_value_field.first,&negative); SetKey(&key,&ascii);
    if(t._M_lower_bound(key)!=&header)return 2;
    puts("STRING_TREE_LOWER_BOUND_PASS");
    return 0;
}