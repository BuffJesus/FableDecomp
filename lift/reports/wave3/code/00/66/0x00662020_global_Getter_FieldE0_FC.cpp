#include <cstddef>
#include <cstdint>

namespace
{
struct Getter_FieldE0_FC_InnerOverlay
{
    std::byte pad00[0xFC];
    std::uint32_t fieldFC; // +0xFC
};
static_assert(offsetof(Getter_FieldE0_FC_InnerOverlay, fieldFC) == 0xFC);

struct Getter_FieldE0_FC_OuterOverlay
{
    std::byte pad00[0xE0];
    Getter_FieldE0_FC_InnerOverlay* fieldE0; // +0xE0
};
static_assert(offsetof(Getter_FieldE0_FC_OuterOverlay, fieldE0) == 0xE0);
}

std::uint32_t __fastcall Getter_FieldE0_FC(const Getter_FieldE0_FC_OuterOverlay* self)
{
    return self->fieldE0->fieldFC;
}