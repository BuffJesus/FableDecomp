#include <stdio.h>

// Behaviour model: Transfer<long> forwards to Transfer<signed_char> with
// (name, (char*)value, (char*)&zeroLocal).

struct Ctx {
    char* got_name;
    char* got_value;
    long  got_extra_deref; // value pointed to by extra (should be 0)

    void Transfer_schar_(char* name, char* value, char* extra) {
        got_name = name;
        got_value = value;
        got_extra_deref = *(long*)extra;
    }
    void Transfer_long_(char* name, long* value) {
        long local = 0;
        this->Transfer_schar_(name, (char*)value, (char*)&local);
    }
};

int main() {
    Ctx c;
    char nm[] = "field";
    long v = 12345;
    c.got_name = 0; c.got_value = 0; c.got_extra_deref = -1;
    c.Transfer_long_(nm, &v);

    int ok = 1;
    if (c.got_name != nm) ok = 0;
    if (c.got_value != (char*)&v) ok = 0;
    if (c.got_extra_deref != 0) ok = 0;

    if (ok) printf("BEHAVIOUR_OK\n");
    else    printf("BEHAVIOUR_FAIL n=%p v=%p e=%ld\n", c.got_name, c.got_value, c.got_extra_deref);
    return 0;
}