#include <cstdlib>
#include <memory>

void* Vector_AllocateAndCopyRGB(int count, CRGBColour* first, CRGBColour* last)
{
    void* result;

    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count << 2);
    }

    if (first != last) {
        auto* dst = static_cast<CRGBColour*>(result);

        do {
            std::construct_at(dst, *first);
            ++dst;
            ++first;
        } while (first != last);
    }

    return result;
}