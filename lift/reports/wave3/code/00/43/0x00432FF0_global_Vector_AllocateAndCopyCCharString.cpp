void* __cdecl Vector_AllocateAndCopyCCharString(
    int count,
    CCharString* first,
    CCharString* last)
{
    void* result;
    int delta;

    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count << 2);
    }

    if (first != last) {
        delta = static_cast<int>(reinterpret_cast<std::uintptr_t>(result)) -
                static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        do {
            const int destAddress =
                static_cast<int>(reinterpret_cast<std::uintptr_t>(first)) + delta;
            CCharString* const dest =
                reinterpret_cast<CCharString*>(destAddress);

            if (dest != nullptr) {
                ::new (dest) CCharString(*first);
            }

            first = reinterpret_cast<CCharString*>(reinterpret_cast<char*>(first) + 4);
        } while (first != last);
    }

    return result;
}