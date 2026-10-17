#include "Lzo.hpp"

#include <fable/codec/Lzo1x.hpp>

#include <cstdint>

#if defined(FABLE_SAVE_HAVE_LZO)
#include <lzo/lzo1x.h>
#endif

namespace fable::save::detail {

std::expected<std::vector<std::byte>, LzoError> lzo1x_decompress(std::span<const std::byte> stream,
                                                                  std::size_t expected_size)
{
    auto out = fable::codec::lzo1x_decompress(stream, expected_size);
    if (!out) {
        return std::unexpected(static_cast<LzoError>(out.error()));   // the two enums share their order
    }
    return std::move(*out);
}

bool lzo_compressor_available() noexcept
{
#if defined(FABLE_SAVE_HAVE_LZO)
    return true;
#else
    return false;
#endif
}

std::optional<std::vector<std::byte>> lzo1x_999_compress(std::span<const std::byte> data)
{
#if defined(FABLE_SAVE_HAVE_LZO)
    // lzo1x_999 never expands by more than size / 16 + 64 + 3 (LZO's documented bound).
    std::vector<std::byte> out(data.size() + data.size() / 16 + 64 + 3);
    std::vector<std::byte> work(LZO1X_999_MEM_COMPRESS);
    lzo_uint out_size = 0;
    const int status = ::lzo1x_999_compress(reinterpret_cast<const unsigned char*>(data.data()),
                                          static_cast<lzo_uint>(data.size()),
                                          reinterpret_cast<unsigned char*>(out.data()), &out_size, work.data());
    if (status != LZO_E_OK) {
        return std::nullopt;
    }
    out.resize(out_size);
    return out;
#else
    (void)data;
    return std::nullopt;
#endif
}

} // namespace fable::save::detail
