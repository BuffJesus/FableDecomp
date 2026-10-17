add_library(fable_texture
    src/GbankTexture.cpp
)

target_include_directories(fable_texture
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(fable_texture PRIVATE fable_codec)

target_compile_features(fable_texture PUBLIC cxx_std_23)
set_target_properties(fable_texture PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    set(FABLE_TEXTURE_WARNINGS /W4 /permissive-)
else()
    set(FABLE_TEXTURE_WARNINGS -Wall -Wextra -Wconversion -Wpedantic)
endif()
target_compile_options(fable_texture PRIVATE ${FABLE_TEXTURE_WARNINGS})

add_executable(fable_texture_dump
    tools/fable_texture_dump.cpp
)
target_link_libraries(fable_texture_dump PRIVATE fable_texture fable_bank)
target_compile_options(fable_texture_dump PRIVATE ${FABLE_TEXTURE_WARNINGS})

# The title viewer presents through the fork's two planned backends: D3D12 (Windows) and Vulkan
# (when the SDK is found). It is Windows-only for now (Win32 window).
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)   # a 64-bit tool: the Vulkan SDK here ships 64-bit libraries
    find_package(Vulkan QUIET)
    add_executable(fable_title_view
        tools/fable_title_view.cpp
        tools/TitleViewD3D12.cpp
    )
    target_link_libraries(fable_title_view PRIVATE fable_texture fable_bank d3d12 dxgi)
    if(Vulkan_FOUND)
        target_sources(fable_title_view PRIVATE tools/TitleViewVulkan.cpp)
        target_link_libraries(fable_title_view PRIVATE Vulkan::Vulkan)
        target_compile_definitions(fable_title_view PRIVATE FABLE_TITLE_VIEW_VULKAN=1)
    endif()
    target_compile_options(fable_title_view PRIVATE ${FABLE_TEXTURE_WARNINGS})
endif()

if(BUILD_TESTING)
    add_executable(fable_texture_tests
        tests/TextureTests.cpp
    )
    target_link_libraries(fable_texture_tests PRIVATE fable_texture)
    target_compile_options(fable_texture_tests PRIVATE ${FABLE_TEXTURE_WARNINGS})

    add_test(NAME fable_texture.Texture
        COMMAND fable_texture_tests
    )
endif()
