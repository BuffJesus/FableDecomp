// Returns a one-word struct by value: the hidden sret pointer is the first stack arg and
// is handed back in eax. __stdcall, sret + one arg (ret 8).
struct W { int value; };
extern "C" int __stdcall Compute(int arg);
W __stdcall MakeWrapper(int arg) {
    W w;
    w.value = Compute(arg);
    return w;
}
