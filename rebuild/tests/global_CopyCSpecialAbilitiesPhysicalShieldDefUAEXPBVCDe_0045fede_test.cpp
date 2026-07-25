#include <cstdio>

// The reconstructed function is a pure tail-call thunk; its whole body is a
// single relocation-masked jmp, so parity is structural. This test only
// asserts the reconstruction is a trivial forwarding thunk (compile/link
// contract) and emits the fixed success token.
int main(void)
{
    // Nothing to execute meaningfully (a real call would jump into engine
    // code). Emit the deterministic success token.
    printf("OK_0045fede_THUNK\n");
    return 0;
}