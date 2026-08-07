// VC7.1's default LIBC directive pulls the CRT startup object even for the
// custom-entry DLL structural smoke. Keep this shim in that link only; the
// executable probe supplies its own real main.
extern "C" int main()
{
    return 0;
}
