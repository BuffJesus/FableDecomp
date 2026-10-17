add_library(fable_codec
    src/Lzo1x.cpp
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
