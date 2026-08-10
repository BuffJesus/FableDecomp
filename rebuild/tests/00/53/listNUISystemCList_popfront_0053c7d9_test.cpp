#include <stdio.h>

struct Node { Node* next; };
struct UIList { Node* head; };

// Behavioural model: pop_front reads this->head->next and hands it to a helper.
static Node* g_seen_node = 0;
static void* g_seen_tmp = 0;

static void helper(Node* n, void* tmp) { g_seen_node = n; g_seen_tmp = tmp; }

static void model_pop_front(UIList* self)
{
    void* tmp = self;               // pushed local holds 'this'
    Node* second = self->head->next; // **this
    helper(second, &tmp);
}

int main(void)
{
    Node a, b;
    a.next = &b;
    b.next = 0;
    UIList list;
    list.head = &a;

    model_pop_front(&list);
    if (g_seen_node != &b) { printf("FAIL node\n"); return 1; }
    if (g_seen_tmp == 0)   { printf("FAIL tmp\n"); return 1; }

    // second branch: empty-ish head->next null
    a.next = 0;
    g_seen_node = (Node*)1;
    model_pop_front(&list);
    if (g_seen_node != 0) { printf("FAIL node2\n"); return 1; }

    printf("OK_0053c7d9\n");
    return 0;
}