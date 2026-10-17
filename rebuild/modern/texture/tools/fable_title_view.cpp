// fable_title_view <frontend.big> [--api d3d12|vulkan] [--seconds N] [--capture <png>]
//
// The modern tree's first frame: decodes the two halves of the retail title logo
// (FRONTEND_TITLE_01_SPRITE, FRONTEND_TITLE_02_SPRITE) with this tree's own bank, LZO1X and
// texture code, composes them over the frontend's black, and presents them in a 64-bit window
// through D3D12 or Vulkan, the fork's two planned backends (MODERNIZATION step 4). --capture
// reads the presented swapchain image back through the same API into a PNG.
//
// This proves the pixel path end to end on both APIs. It is not the fork's renderer: no RHI,
// no shaders, just a copy into the swapchain image each frame.

#include "TitleView.hpp"

#include <fable/bank/BigBank.hpp>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace title_view {

bool pump_messages()
{
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}

void write_png(const std::filesystem::path& path, const fable::texture::Image& image)
{
    const auto png = fable::texture::encode_png(image);
    std::ofstream(path, std::ios::binary).write(reinterpret_cast<const char*>(png.data()), static_cast<std::streamsize>(png.size()));
}

#if !defined(FABLE_TITLE_VIEW_VULKAN)
std::string run_vulkan(HWND, HINSTANCE, const fable::texture::Image&, const Options&)
{
    return "this build has no Vulkan (the Vulkan SDK was not found at configure time)";
}
#endif

} // namespace title_view

namespace {

std::shared_ptr<const std::vector<std::byte>> read_file(const std::filesystem::path& path)
{
    std::ifstream in(path, std::ios::binary);
    auto bytes = std::make_shared<std::vector<std::byte>>(static_cast<std::size_t>(std::filesystem::file_size(path)));
    in.read(reinterpret_cast<char*>(bytes->data()), static_cast<std::streamsize>(bytes->size()));
    return bytes;
}

std::optional<fable::texture::Image> load_sprite(const fable::bank::BigBank& bank, const std::string& name)
{
    for (const auto& sub : bank.banks) {
        for (const auto& entry : sub.entries) {
            if (entry.name_text() != name) {
                continue;
            }
            const auto info = fable::texture::TextureInfo::parse(entry.info);
            if (!info) {
                return std::nullopt;
            }
            const auto surface = fable::texture::decode_mip0(*info, entry.payload.bytes());
            if (!surface) {
                return std::nullopt;
            }
            auto image = fable::texture::to_rgba(*info, *surface);
            return image ? std::optional(std::move(*image)) : std::nullopt;
        }
    }
    return std::nullopt;
}

// The window-sized frame: black, with the two halves side by side at (margin, margin),
// alpha-blended over the black like the frontend draws them.
fable::texture::Image compose(const fable::texture::Image& left, const fable::texture::Image& right, std::uint32_t margin)
{
    fable::texture::Image out;
    out.width = left.width + right.width + 2 * margin;
    out.height = std::max(left.height, right.height) + 2 * margin;
    out.rgba.assign(static_cast<std::size_t>(out.width) * out.height * 4, 0);
    for (std::size_t i = 3; i < out.rgba.size(); i += 4) {
        out.rgba[i] = 255;
    }
    auto blit = [&](const fable::texture::Image& src, std::uint32_t x0) {
        for (std::uint32_t y = 0; y < src.height; ++y) {
            for (std::uint32_t x = 0; x < src.width; ++x) {
                const auto* s = &src.rgba[(static_cast<std::size_t>(y) * src.width + x) * 4];
                auto* d = &out.rgba[(static_cast<std::size_t>(y + margin) * out.width + x0 + x) * 4];
                for (int c = 0; c < 3; ++c) {
                    d[c] = static_cast<std::uint8_t>(s[c] * s[3] / 255);
                }
            }
        }
    };
    blit(left, margin);
    blit(right, margin + left.width);
    return out;
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, w, l);
}

} // namespace

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: fable_title_view <frontend.big> [--api d3d12|vulkan] [--seconds N] [--capture <png>]\n";
        return 2;
    }
    std::string api = "d3d12";
    title_view::Options options;
    for (int i = 2; i + 1 < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--api") {
            api = argv[++i];
        } else if (arg == "--seconds") {
            options.seconds = std::stod(argv[++i]);
        } else if (arg == "--capture") {
            options.capture = argv[++i];
        }
    }
    if (api != "d3d12" && api != "vulkan") {
        std::cerr << "--api must be d3d12 or vulkan\n";
        return 2;
    }

    const auto bank = fable::bank::BigBank::decode(read_file(argv[1]));
    if (!bank) {
        std::cerr << "not a BIGB bank\n";
        return 1;
    }
    const auto left = load_sprite(*bank, "FRONTEND_TITLE_01_SPRITE");
    const auto right = load_sprite(*bank, "FRONTEND_TITLE_02_SPRITE");
    if (!left || !right) {
        std::cerr << "title sprites not found or not decodable\n";
        return 1;
    }
    const auto frame = compose(*left, *right, 32);
    std::cout << "decoded title " << left->width + right->width << "x" << left->height << "; presenting through " << api << '\n';

    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"FableTitleView";
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));   // IDC_ARROW
    RegisterClassW(&wc);
    RECT rc{0, 0, static_cast<LONG>(frame.width), static_cast<LONG>(frame.height)};
    const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;   // fixed size, like the frame
    AdjustWindowRect(&rc, style, FALSE);
    const std::wstring title = api == "d3d12" ? L"Fable: The Lost Chapters - modern tree (x64, D3D12)"
                                              : L"Fable: The Lost Chapters - modern tree (x64, Vulkan)";
    HWND hwnd = CreateWindowW(wc.lpszClassName, title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
                              rc.bottom - rc.top, nullptr, nullptr, wc.hInstance, nullptr);
    ShowWindow(hwnd, SW_SHOW);

    const std::string error = api == "d3d12" ? title_view::run_d3d12(hwnd, frame, options)
                                             : title_view::run_vulkan(hwnd, wc.hInstance, frame, options);
    if (!error.empty()) {
        std::cerr << api << ": " << error << '\n';
        return 1;
    }
    return 0;
}
