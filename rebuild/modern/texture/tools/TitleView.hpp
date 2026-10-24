#pragma once

// Shared by the fable_title_view backends. The frame is composed on the CPU (the title over
// black, window-sized), so each backend only uploads, copies into its swapchain image and
// presents; --capture reads that swapchain image back through the same API.

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <fable/texture/GbankTexture.hpp>

#include <filesystem>
#include <optional>
#include <string>

namespace title_view {

struct Options {
    double seconds = 0;                                // 0 = until the window closes
    std::optional<std::filesystem::path> capture;      // PNG of the presented swapchain image
};

// Runs the window's message loop and presents `frame` every vsync until it closes or
// options.seconds pass. Returns an error description, or "" on success.
std::string run_d3d12(HWND hwnd, const fable::texture::Image& frame, const Options& options);
std::string run_vulkan(HWND hwnd, HINSTANCE instance, const fable::texture::Image& frame, const Options& options);

// Pumps messages; false once the window has been destroyed.
bool pump_messages();

void write_png(const std::filesystem::path& path, const fable::texture::Image& image);

} // namespace title_view
