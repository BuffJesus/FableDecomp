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

# The title viewer presents through D3D11, so it is Windows-only.
if(WIN32)
    add_executable(fable_title_view
        tools/fable_title_view.cpp
    )
    target_link_libraries(fable_title_view PRIVATE fable_texture fable_bank d3d11)
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
