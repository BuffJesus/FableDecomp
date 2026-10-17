// fable_title_view <frontend.big> [--seconds N] [--capture <png>]
//
// The modern tree's first frame: decodes the two halves of the retail title logo
// (FRONTEND_TITLE_01_SPRITE, FRONTEND_TITLE_02_SPRITE) with this tree's own bank, LZO1X and
// texture code, composes them over the frontend's black, and presents them in a 64-bit window
// through D3D11. --capture reads the presented back buffer back from the GPU into a PNG.
//
// This is a viewer, not the fork's renderer (MODERNIZATION step 4 plans D3D12 + Vulkan behind an
// RHI): it only clears and copies, so it needs no shaders.

#include <fable/bank/BigBank.hpp>
#include <fable/texture/GbankTexture.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace {

template <typename T>
struct Com {
    T* p = nullptr;
    Com() = default;
    Com(const Com&) = delete;
    Com& operator=(const Com&) = delete;
    ~Com()
    {
        if (p) {
            p->Release();
        }
    }
    T** put() { return &p; }
    T* operator->() const { return p; }
    explicit operator bool() const { return p != nullptr; }
};

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

// The two halves side by side, alpha-blended over black (the frontend's backdrop colour).
fable::texture::Image compose(const fable::texture::Image& left, const fable::texture::Image& right)
{
    fable::texture::Image out;
    out.width = left.width + right.width;
    out.height = std::max(left.height, right.height);
    out.rgba.assign(static_cast<std::size_t>(out.width) * out.height * 4, 0);
    auto blit = [&](const fable::texture::Image& src, std::uint32_t x0) {
        for (std::uint32_t y = 0; y < src.height; ++y) {
            for (std::uint32_t x = 0; x < src.width; ++x) {
                const auto* s = &src.rgba[(static_cast<std::size_t>(y) * src.width + x) * 4];
                auto* d = &out.rgba[(static_cast<std::size_t>(y) * out.width + x0 + x) * 4];
                for (int c = 0; c < 3; ++c) {
                    d[c] = static_cast<std::uint8_t>(s[c] * s[3] / 255);
                }
                d[3] = 255;
            }
        }
    };
    blit(left, 0);
    blit(right, left.width);
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

void write_png(const std::filesystem::path& path, const fable::texture::Image& image)
{
    const auto png = fable::texture::encode_png(image);
    std::ofstream(path, std::ios::binary).write(reinterpret_cast<const char*>(png.data()), static_cast<std::streamsize>(png.size()));
}

} // namespace

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: fable_title_view <frontend.big> [--seconds N] [--capture <png>]\n";
        return 2;
    }
    double seconds = 0;
    std::optional<std::filesystem::path> capture;
    for (int i = 2; i + 1 < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--seconds") {
            seconds = std::stod(argv[++i]);
        } else if (arg == "--capture") {
            capture = argv[++i];
        }
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
    const auto title = compose(*left, *right);
    std::cout << "decoded title " << title.width << "x" << title.height << " from " << argv[1] << '\n';

    // Window: the title centred with a margin.
    const UINT width = title.width + 64;
    const UINT height = title.height + 64;
    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"FableTitleView";
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));   // IDC_ARROW
    RegisterClassW(&wc);
    RECT rc{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Fable: The Lost Chapters - modern tree (x64, D3D11)", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr,
                              wc.hInstance, nullptr);
    ShowWindow(hwnd, SW_SHOW);

    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    Com<IDXGISwapChain> swap;
    Com<ID3D11Device> device;
    Com<ID3D11DeviceContext> ctx;
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd,
                                             swap.put(), device.put(), nullptr, ctx.put()))) {
        std::cerr << "D3D11 device creation failed\n";
        return 1;
    }

    D3D11_TEXTURE2D_DESC td{};
    td.Width = title.width;
    td.Height = title.height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_IMMUTABLE;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    D3D11_SUBRESOURCE_DATA init{title.rgba.data(), title.width * 4, 0};
    Com<ID3D11Texture2D> sprite;
    if (FAILED(device->CreateTexture2D(&td, &init, sprite.put()))) {
        std::cerr << "texture upload failed\n";
        return 1;
    }

    const auto start = std::chrono::steady_clock::now();
    bool captured = false;
    MSG msg{};
    for (;;) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return 0;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        Com<ID3D11Texture2D> back;
        swap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back.put()));
        Com<ID3D11RenderTargetView> rtv;
        device->CreateRenderTargetView(back.p, nullptr, rtv.put());
        const float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        ctx->ClearRenderTargetView(rtv.p, black);
        ctx->CopySubresourceRegion(back.p, 0, 32, 32, 0, sprite.p, 0, nullptr);

        if (capture && !captured) {
            // Read the composed back buffer from the GPU, before Present discards it.
            D3D11_TEXTURE2D_DESC bd{};
            back->GetDesc(&bd);
            bd.Usage = D3D11_USAGE_STAGING;
            bd.BindFlags = 0;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            bd.MiscFlags = 0;
            Com<ID3D11Texture2D> staging;
            device->CreateTexture2D(&bd, nullptr, staging.put());
            ctx->CopyResource(staging.p, back.p);
            D3D11_MAPPED_SUBRESOURCE m{};
            if (SUCCEEDED(ctx->Map(staging.p, 0, D3D11_MAP_READ, 0, &m))) {
                fable::texture::Image shot;
                shot.width = bd.Width;
                shot.height = bd.Height;
                shot.rgba.resize(static_cast<std::size_t>(bd.Width) * bd.Height * 4);
                for (UINT y = 0; y < bd.Height; ++y) {
                    std::memcpy(&shot.rgba[static_cast<std::size_t>(y) * bd.Width * 4],
                                static_cast<const std::uint8_t*>(m.pData) + static_cast<std::size_t>(y) * m.RowPitch, bd.Width * 4);
                }
                ctx->Unmap(staging.p, 0);
                write_png(*capture, shot);
                std::cout << "captured the presented frame to " << capture->string() << '\n';
            }
            captured = true;
        }
        swap->Present(1, 0);

        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (seconds > 0 && elapsed > seconds) {
            DestroyWindow(hwnd);
        }
    }
}
