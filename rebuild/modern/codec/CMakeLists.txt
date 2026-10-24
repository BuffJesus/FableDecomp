add_library(fable_codec
    src/Lzo1x.cpp
    src/Lzo1xCompress.cpp
)

target_include_directories(fable_codec
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(fable_codec PUBLIC cxx_std_23)
set_target_properties(fable_codec PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(fable_codec PRIVATE /W4 /permissive-)
else()
    target_compile_options(fable_codec PRIVATE -Wall -Wextra -Wconversion -Wpedantic)
endif()

if(BUILD_TESTING)
    add_executable(fable_codec_tests
        tests/Lzo1xTests.cpp
    )
    target_link_libraries(fable_codec_tests PRIVATE fable_codec)
    add_test(NAME fable_codec.Lzo1x
        COMMAND fable_codec_tests
    )
endif()
