// D3D12 backend for fable_title_view: an upload buffer with the frame (rows padded to the
// 256-byte copy pitch), copied into the current back buffer each frame and presented.

#include "TitleView.hpp"

#include <d3d12.h>
#include <dxgi1_4.h>

#include <chrono>
#include <cstdint>
#include <cstring>
#include <vector>

namespace title_view {
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
};

constexpr UINT buffer_count = 2;

D3D12_RESOURCE_BARRIER transition(ID3D12Resource* r, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)
{
    D3D12_RESOURCE_BARRIER b{};
    b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    b.Transition.pResource = r;
    b.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    b.Transition.StateBefore = from;
    b.Transition.StateAfter = to;
    return b;
}

ID3D12Resource* make_buffer(ID3D12Device* device, D3D12_HEAP_TYPE heap, UINT64 size, D3D12_RESOURCE_STATES state)
{
    D3D12_HEAP_PROPERTIES hp{};
    hp.Type = heap;
    D3D12_RESOURCE_DESC rd{};
    rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    rd.Width = size;
    rd.Height = 1;
    rd.DepthOrArraySize = 1;
    rd.MipLevels = 1;
    rd.SampleDesc.Count = 1;
    rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    ID3D12Resource* r = nullptr;
    device->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, state, nullptr, __uuidof(ID3D12Resource),
                                    reinterpret_cast<void**>(&r));
    return r;
}

} // namespace

std::string run_d3d12(HWND hwnd, const fable::texture::Image& frame, const Options& options)
{
    Com<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory4), reinterpret_cast<void**>(factory.put())))) {
        return "CreateDXGIFactory1 failed";
    }
    Com<ID3D12Device> device;
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), reinterpret_cast<void**>(device.put())))) {
        return "D3D12CreateDevice failed";
    }
    D3D12_COMMAND_QUEUE_DESC qd{};
    qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    Com<ID3D12CommandQueue> queue;
    device->CreateCommandQueue(&qd, __uuidof(ID3D12CommandQueue), reinterpret_cast<void**>(queue.put()));

    DXGI_SWAP_CHAIN_DESC1 sd{};
    sd.Width = frame.width;
    sd.Height = frame.height;
    sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = buffer_count;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    Com<IDXGISwapChain1> swap1;
    if (FAILED(factory->CreateSwapChainForHwnd(queue.p, hwnd, &sd, nullptr, nullptr, swap1.put()))) {
        return "CreateSwapChainForHwnd failed";
    }
    Com<IDXGISwapChain3> swap;
    swap1->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(swap.put()));
    Com<ID3D12Resource> backs[buffer_count];
    for (UINT i = 0; i < buffer_count; ++i) {
        swap->GetBuffer(i, __uuidof(ID3D12Resource), reinterpret_cast<void**>(backs[i].put()));
    }

    // Upload and readback buffers share the copy footprint of a back buffer.
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
    UINT64 total = 0;
    const D3D12_RESOURCE_DESC back_desc = backs[0]->GetDesc();
    device->GetCopyableFootprints(&back_desc, 0, 1, 0, &footprint, nullptr, nullptr, &total);
    Com<ID3D12Resource> upload;
    upload.p = make_buffer(device.p, D3D12_HEAP_TYPE_UPLOAD, total, D3D12_RESOURCE_STATE_GENERIC_READ);
    Com<ID3D12Resource> readback;
    readback.p = make_buffer(device.p, D3D12_HEAP_TYPE_READBACK, total, D3D12_RESOURCE_STATE_COPY_DEST);
    if (!upload.p || !readback.p) {
        return "buffer creation failed";
    }
    std::uint8_t* mapped = nullptr;
    upload->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
    for (UINT y = 0; y < frame.height; ++y) {
        std::memcpy(mapped + static_cast<std::size_t>(y) * footprint.Footprint.RowPitch,
                    &frame.rgba[static_cast<std::size_t>(y) * frame.width * 4], frame.width * 4);
    }
    upload->Unmap(0, nullptr);

    Com<ID3D12CommandAllocator> alloc;
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), reinterpret_cast<void**>(alloc.put()));
    Com<ID3D12GraphicsCommandList> list;
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, alloc.p, nullptr, __uuidof(ID3D12GraphicsCommandList),
                              reinterpret_cast<void**>(list.put()));
    list->Close();
    Com<ID3D12Fence> fence;
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), reinterpret_cast<void**>(fence.put()));
    HANDLE event = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    UINT64 fence_value = 0;
    auto wait_idle = [&] {
        queue->Signal(fence.p, ++fence_value);
        if (fence->GetCompletedValue() < fence_value) {
            fence->SetEventOnCompletion(fence_value, event);
            WaitForSingleObject(event, INFINITE);
        }
    };

    const auto start = std::chrono::steady_clock::now();
    bool captured = false;
    while (pump_messages()) {
        const UINT index = swap->GetCurrentBackBufferIndex();
        ID3D12Resource* back = backs[index].p;
        alloc->Reset();
        list->Reset(alloc.p, nullptr);
        auto to_copy = transition(back, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
        list->ResourceBarrier(1, &to_copy);
        D3D12_TEXTURE_COPY_LOCATION dst{};
        dst.pResource = back;
        dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        D3D12_TEXTURE_COPY_LOCATION src{};
        src.pResource = upload.p;
        src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.PlacedFootprint = footprint;
        list->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        const bool capture_now = options.capture && !captured;
        if (capture_now) {   // read the composed back buffer before it is presented
            auto to_src = transition(back, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE);
            list->ResourceBarrier(1, &to_src);
            D3D12_TEXTURE_COPY_LOCATION rb{};
            rb.pResource = readback.p;
            rb.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            rb.PlacedFootprint = footprint;
            list->CopyTextureRegion(&rb, 0, 0, 0, &dst, nullptr);
            auto to_present = transition(back, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_PRESENT);
            list->ResourceBarrier(1, &to_present);
        } else {
            auto to_present = transition(back, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
            list->ResourceBarrier(1, &to_present);
        }
        list->Close();
        ID3D12CommandList* lists[] = {list.p};
        queue->ExecuteCommandLists(1, lists);
        swap->Present(1, 0);
        wait_idle();

        if (capture_now) {
            std::uint8_t* data = nullptr;
            readback->Map(0, nullptr, reinterpret_cast<void**>(&data));
            fable::texture::Image shot;
            shot.width = frame.width;
            shot.height = frame.height;
            shot.rgba.resize(frame.rgba.size());
            for (UINT y = 0; y < frame.height; ++y) {
                std::memcpy(&shot.rgba[static_cast<std::size_t>(y) * frame.width * 4],
                            data + static_cast<std::size_t>(y) * footprint.Footprint.RowPitch, frame.width * 4);
            }
            readback->Unmap(0, nullptr);
            write_png(*options.capture, shot);
            captured = true;
        }
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (options.seconds > 0 && elapsed > options.seconds) {
            DestroyWindow(hwnd);
        }
    }
    wait_idle();
    CloseHandle(event);
    return "";
}

} // namespace title_view
