static_assert(sizeof(CWideString) == 4);

CWideString* __fastcall CWideString_FillBackward(CWideString* first, CWideString* last, CWideString* result)
{
    std::intptr_t count =
        (static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(last)) -
         static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(first))) >> 2;

    if (0 < count)
    {
        const std::intptr_t delta =
            static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(last)) -
            static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(result));

        do
        {
            result = reinterpret_cast<CWideString*>(
                reinterpret_cast<std::uint8_t*>(result) - 4);

            result->CWideString::operator=(
                *reinterpret_cast<CWideString*>(
                    reinterpret_cast<std::uint8_t*>(result) + delta));

            count = count - 1;
        } while (count != 0);
    }

    return result;
}