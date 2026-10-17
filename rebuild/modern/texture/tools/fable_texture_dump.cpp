// fable_texture_dump <bank.big> [--check] [--out <dir>] [ENTRY_NAME...]
//
// Decodes GBANK texture entries (mip 0 of frame 0) to PNG files named <ENTRY_NAME>.png.
// --check decodes every texture entry of every GBANK sub-bank and reports the totals by pixel
// format; exit status 1 if any entry fails.

#include <fable/bank/BigBank.hpp>
#include <fable/texture/GbankTexture.hpp>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace {

std::shared_ptr<const std::vector<std::byte>> read_file(const std::filesystem::path& path)
{
    std::ifstream in(path, std::ios::binary);
    auto bytes = std::make_shared<std::vector<std::byte>>(static_cast<std::size_t>(std::filesystem::file_size(path)));
    in.read(reinterpret_cast<char*>(bytes->data()), static_cast<std::streamsize>(bytes->size()));
    return bytes;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: fable_texture_dump <bank.big> [--check] [--out <dir>] [ENTRY_NAME...]\n";
        return 2;
    }
    bool check = false;
    std::filesystem::path out_dir = ".";
    std::set<std::string> wanted;
    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--check") {
            check = true;
        } else if (arg == "--out" && i + 1 < argc) {
            out_dir = argv[++i];
        } else {
            wanted.insert(arg);
        }
    }
    const auto bank = fable::bank::BigBank::decode(read_file(argv[1]));
    if (!bank) {
        std::cerr << "not a BIGB bank\n";
        return 1;
    }
    std::map<std::string, int> totals;
    int failures = 0;
    for (const auto& sub : bank->banks) {
        if (sub.name.rfind("GBANK", 0) != 0) {
            continue;
        }
        for (const auto& entry : sub.entries) {
            const std::string name(entry.name_text());
            if (!check && !wanted.contains(name)) {
                continue;
            }
            const auto info = fable::texture::TextureInfo::parse(entry.info);
            std::string verdict;
            if (!info) {
                verdict = "bad_info";
            } else {
                const auto surface = fable::texture::decode_mip0(*info, entry.payload.bytes());
                const auto image = surface ? fable::texture::to_rgba(*info, *surface)
                                           : std::expected<fable::texture::Image, fable::texture::TextureError>(
                                                 std::unexpected(surface.error()));
                if (!image) {
                    verdict = image.error() == fable::texture::TextureError::unsupported_format ? "unsupported_format"
                                                                                                 : "bad_payload";
                } else {
                    verdict = "ok";
                    if (wanted.contains(name)) {
                        std::filesystem::create_directories(out_dir);
                        const auto png = fable::texture::encode_png(*image);
                        std::ofstream(out_dir / (name + ".png"), std::ios::binary)
                            .write(reinterpret_cast<const char*>(png.data()), static_cast<std::streamsize>(png.size()));
                        std::cout << "wrote " << (out_dir / (name + ".png")).string() << " (" << image->width << "x"
                                  << image->height << ")\n";
                    }
                }
            }
            const auto fmt = info ? info->pixel_format : 0U;
            ++totals[verdict + " fmt=" + std::to_string(fmt)];
            if (verdict != "ok" && verdict != "unsupported_format") {
                ++failures;
                if (failures <= 10) {
                    std::cout << verdict << '\t' << sub.name << '\t' << name << '\n';
                }
            }
        }
    }
    if (check) {
        for (const auto& [k, v] : totals) {
            std::cout << v << '\t' << k << '\n';
        }
    }
    return failures == 0 ? 0 : 1;
}
