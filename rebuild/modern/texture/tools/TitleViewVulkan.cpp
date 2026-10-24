// Vulkan backend for fable_title_view: a host-visible staging buffer with the frame, copied into
// the acquired swapchain image each frame and presented.

#include "TitleView.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

namespace title_view {
namespace {

struct Vk {
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkBuffer staging = VK_NULL_HANDLE, readback = VK_NULL_HANDLE;
    VkDeviceMemory staging_mem = VK_NULL_HANDLE, readback_mem = VK_NULL_HANDLE;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkSemaphore acquired = VK_NULL_HANDLE, done = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;

    ~Vk()
    {
        if (device) {
            vkDeviceWaitIdle(device);
            vkDestroyFence(device, fence, nullptr);
            vkDestroySemaphore(device, done, nullptr);
            vkDestroySemaphore(device, acquired, nullptr);
            vkDestroyCommandPool(device, pool, nullptr);
            vkDestroyBuffer(device, readback, nullptr);
            vkFreeMemory(device, readback_mem, nullptr);
            vkDestroyBuffer(device, staging, nullptr);
            vkFreeMemory(device, staging_mem, nullptr);
            vkDestroySwapchainKHR(device, swapchain, nullptr);
            vkDestroyDevice(device, nullptr);
        }
        if (instance) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
        }
    }
};

bool make_buffer(VkPhysicalDevice gpu, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer& buffer,
                 VkDeviceMemory& memory)
{
    VkBufferCreateInfo bi{};
    bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bi.size = size;
    bi.usage = usage;
    if (vkCreateBuffer(device, &bi, nullptr, &buffer) != VK_SUCCESS) {
        return false;
    }
    VkMemoryRequirements req{};
    vkGetBufferMemoryRequirements(device, buffer, &req);
    VkPhysicalDeviceMemoryProperties props{};
    vkGetPhysicalDeviceMemoryProperties(gpu, &props);
    const VkMemoryPropertyFlags want = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    for (uint32_t i = 0; i < props.memoryTypeCount; ++i) {
        if ((req.memoryTypeBits & (1U << i)) && (props.memoryTypes[i].propertyFlags & want) == want) {
            VkMemoryAllocateInfo ai{};
            ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            ai.allocationSize = req.size;
            ai.memoryTypeIndex = i;
            return vkAllocateMemory(device, &ai, nullptr, &memory) == VK_SUCCESS &&
                   vkBindBufferMemory(device, buffer, memory, 0) == VK_SUCCESS;
        }
    }
    return false;
}

void barrier(VkCommandBuffer cmd, VkImage image, VkImageLayout from, VkImageLayout to, VkAccessFlags src_access,
             VkAccessFlags dst_access)
{
    VkImageMemoryBarrier b{};
    b.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    b.oldLayout = from;
    b.newLayout = to;
    b.srcAccessMask = src_access;
    b.dstAccessMask = dst_access;
    b.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = image;
    b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0,
                         nullptr, 1, &b);
}

} // namespace

std::string run_vulkan(HWND hwnd, HINSTANCE instance, const fable::texture::Image& frame, const Options& options)
{
    Vk vk;
    VkApplicationInfo app{};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "fable_title_view";
    app.apiVersion = VK_API_VERSION_1_1;
    const char* instance_ext[] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
    VkInstanceCreateInfo ici{};
    ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pApplicationInfo = &app;
    ici.enabledExtensionCount = 2;
    ici.ppEnabledExtensionNames = instance_ext;
    if (vkCreateInstance(&ici, nullptr, &vk.instance) != VK_SUCCESS) {
        return "vkCreateInstance failed";
    }
    VkWin32SurfaceCreateInfoKHR sci{};
    sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    sci.hinstance = instance;
    sci.hwnd = hwnd;
    if (vkCreateWin32SurfaceKHR(vk.instance, &sci, nullptr, &vk.surface) != VK_SUCCESS) {
        return "vkCreateWin32SurfaceKHR failed";
    }

    uint32_t gpu_count = 0;
    vkEnumeratePhysicalDevices(vk.instance, &gpu_count, nullptr);
    std::vector<VkPhysicalDevice> gpus(gpu_count);
    vkEnumeratePhysicalDevices(vk.instance, &gpu_count, gpus.data());
    VkPhysicalDevice gpu = VK_NULL_HANDLE;
    uint32_t family = 0;
    for (VkPhysicalDevice g : gpus) {
        uint32_t n = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(g, &n, nullptr);
        std::vector<VkQueueFamilyProperties> fams(n);
        vkGetPhysicalDeviceQueueFamilyProperties(g, &n, fams.data());
        for (uint32_t f = 0; f < n && gpu == VK_NULL_HANDLE; ++f) {
            VkBool32 present = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(g, f, vk.surface, &present);
            if ((fams[f].queueFlags & VK_QUEUE_GRAPHICS_BIT) && present) {
                gpu = g;
                family = f;
            }
        }
        if (gpu != VK_NULL_HANDLE) {
            break;
        }
    }
    if (gpu == VK_NULL_HANDLE) {
        return "no GPU with a graphics queue that can present to the window";
    }
    VkPhysicalDeviceProperties gpu_props{};
    vkGetPhysicalDeviceProperties(gpu, &gpu_props);

    const float priority = 1.0f;
    VkDeviceQueueCreateInfo qci{};
    qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qci.queueFamilyIndex = family;
    qci.queueCount = 1;
    qci.pQueuePriorities = &priority;
    const char* device_ext[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    dci.enabledExtensionCount = 1;
    dci.ppEnabledExtensionNames = device_ext;
    if (vkCreateDevice(gpu, &dci, nullptr, &vk.device) != VK_SUCCESS) {
        return "vkCreateDevice failed";
    }
    VkQueue queue = VK_NULL_HANDLE;
    vkGetDeviceQueue(vk.device, family, 0, &queue);

    // Swapchain: an 8-bit UNORM format (RGBA or BGRA), images usable as copy targets and sources.
    uint32_t nfmt = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, vk.surface, &nfmt, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(nfmt);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, vk.surface, &nfmt, formats.data());
    VkSurfaceFormatKHR fmt{VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    for (const auto& f : formats) {
        if (f.format == VK_FORMAT_R8G8B8A8_UNORM || f.format == VK_FORMAT_B8G8R8A8_UNORM) {
            fmt = f;
            break;
        }
    }
    if (fmt.format == VK_FORMAT_UNDEFINED) {
        return "no 8-bit UNORM swapchain format";
    }
    const bool bgra = fmt.format == VK_FORMAT_B8G8R8A8_UNORM;
    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, vk.surface, &caps);
    if (!(caps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
        return "swapchain images cannot be copy targets";
    }
    VkSwapchainCreateInfoKHR swci{};
    swci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swci.surface = vk.surface;
    swci.minImageCount = std::max(2U, caps.minImageCount);
    swci.imageFormat = fmt.format;
    swci.imageColorSpace = fmt.colorSpace;
    swci.imageExtent = {frame.width, frame.height};
    swci.imageArrayLayers = 1;
    swci.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                      (caps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
    swci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swci.preTransform = caps.currentTransform;
    swci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swci.presentMode = VK_PRESENT_MODE_FIFO_KHR;   // vsync; always available
    swci.clipped = VK_TRUE;
    if (vkCreateSwapchainKHR(vk.device, &swci, nullptr, &vk.swapchain) != VK_SUCCESS) {
        return "vkCreateSwapchainKHR failed";
    }
    const bool can_capture = (swci.imageUsage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) != 0;
    uint32_t nimg = 0;
    vkGetSwapchainImagesKHR(vk.device, vk.swapchain, &nimg, nullptr);
    std::vector<VkImage> images(nimg);
    vkGetSwapchainImagesKHR(vk.device, vk.swapchain, &nimg, images.data());

    // Staging data in the swapchain's channel order.
    const VkDeviceSize size = frame.rgba.size();
    if (!make_buffer(gpu, vk.device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, vk.staging, vk.staging_mem) ||
        !make_buffer(gpu, vk.device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, vk.readback, vk.readback_mem)) {
        return "buffer creation failed";
    }
    void* mapped = nullptr;
    vkMapMemory(vk.device, vk.staging_mem, 0, size, 0, &mapped);
    auto* dst = static_cast<std::uint8_t*>(mapped);
    std::memcpy(dst, frame.rgba.data(), frame.rgba.size());
    if (bgra) {
        for (std::size_t i = 0; i < frame.rgba.size(); i += 4) {
            std::swap(dst[i], dst[i + 2]);
        }
    }
    vkUnmapMemory(vk.device, vk.staging_mem);

    VkCommandPoolCreateInfo pci{};

    pci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pci.queueFamilyIndex = family;
    vkCreateCommandPool(vk.device, &pci, nullptr, &vk.pool);
    VkCommandBufferAllocateInfo cai{};
    cai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cai.commandPool = vk.pool;
    cai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cai.commandBufferCount = 1;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    vkAllocateCommandBuffers(vk.device, &cai, &cmd);
    VkSemaphoreCreateInfo semi{};
    semi.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(vk.device, &semi, nullptr, &vk.acquired);
    vkCreateSemaphore(vk.device, &semi, nullptr, &vk.done);
    VkFenceCreateInfo fci{};
    fci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkCreateFence(vk.device, &fci, nullptr, &vk.fence);

    VkBufferImageCopy region{};
    region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    region.imageExtent = {frame.width, frame.height, 1};

    const auto start = std::chrono::steady_clock::now();
    bool captured = false;
    while (pump_messages()) {
        uint32_t index = 0;
        if (vkAcquireNextImageKHR(vk.device, vk.swapchain, UINT64_MAX, vk.acquired, VK_NULL_HANDLE, &index) != VK_SUCCESS) {
            break;
        }
        const VkImage image = images[index];
        const bool capture_now = options.capture && !captured && can_capture;
        vkResetCommandBuffer(cmd, 0);
        VkCommandBufferBeginInfo bi{};
        bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmd, &bi);
        barrier(cmd, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT);
        vkCmdCopyBufferToImage(cmd, vk.staging, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        VkImageLayout layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        if (capture_now) {
            barrier(cmd, image, layout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_ACCESS_TRANSFER_WRITE_BIT,
                    VK_ACCESS_TRANSFER_READ_BIT);
            layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            vkCmdCopyImageToBuffer(cmd, image, layout, vk.readback, 1, &region);
        }
        barrier(cmd, image, layout, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_TRANSFER_READ_BIT, 0);
        vkEndCommandBuffer(cmd);

        const VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        VkSubmitInfo si{};
        si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        si.waitSemaphoreCount = 1;
        si.pWaitSemaphores = &vk.acquired;
        si.pWaitDstStageMask = &wait_stage;
        si.commandBufferCount = 1;
        si.pCommandBuffers = &cmd;
        si.signalSemaphoreCount = 1;
        si.pSignalSemaphores = &vk.done;
        vkQueueSubmit(queue, 1, &si, vk.fence);
        VkPresentInfoKHR pi{};
        pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        pi.waitSemaphoreCount = 1;
        pi.pWaitSemaphores = &vk.done;
        pi.swapchainCount = 1;
        pi.pSwapchains = &vk.swapchain;
        pi.pImageIndices = &index;
        vkQueuePresentKHR(queue, &pi);
        vkWaitForFences(vk.device, 1, &vk.fence, VK_TRUE, UINT64_MAX);
        vkResetFences(vk.device, 1, &vk.fence);

        if (capture_now) {
            void* data = nullptr;
            vkMapMemory(vk.device, vk.readback_mem, 0, size, 0, &data);
            fable::texture::Image shot;
            shot.width = frame.width;
            shot.height = frame.height;
            shot.rgba.assign(static_cast<const std::uint8_t*>(data), static_cast<const std::uint8_t*>(data) + size);
            vkUnmapMemory(vk.device, vk.readback_mem);
            if (bgra) {
                for (std::size_t i = 0; i < shot.rgba.size(); i += 4) {
                    std::swap(shot.rgba[i], shot.rgba[i + 2]);
                }
            }
            write_png(*options.capture, shot);
            captured = true;
        }
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        if (options.seconds > 0 && elapsed > options.seconds) {
            DestroyWindow(hwnd);
        }
    }
    if (options.capture && !captured) {
        return can_capture ? "window closed before a frame was captured" : "swapchain images cannot be read back";
    }
    (void)gpu_props;
    return "";
}

} // namespace title_view
