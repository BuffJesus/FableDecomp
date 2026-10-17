# zlib: an installed package, a local source tree (-DFABLE_ZLIB_SOURCE_DIR=...), or a pinned
# download. Use classic zlib (1.2.x/1.3.x). zlib-ng deflates differently and would break the
# byte-identical save rewrite.
set(FABLE_ZLIB_SOURCE_DIR "" CACHE PATH "Classic zlib source tree to build instead of find_package(ZLIB)")

if(FABLE_ZLIB_SOURCE_DIR)
    add_library(fable_zlib STATIC
        "${FABLE_ZLIB_SOURCE_DIR}/adler32.c"
        "${FABLE_ZLIB_SOURCE_DIR}/crc32.c"
        "${FABLE_ZLIB_SOURCE_DIR}/deflate.c"
        "${FABLE_ZLIB_SOURCE_DIR}/inffast.c"
        "${FABLE_ZLIB_SOURCE_DIR}/inflate.c"
        "${FABLE_ZLIB_SOURCE_DIR}/inftrees.c"
        "${FABLE_ZLIB_SOURCE_DIR}/trees.c"
        "${FABLE_ZLIB_SOURCE_DIR}/zutil.c"
    )
    target_include_directories(fable_zlib PUBLIC "${FABLE_ZLIB_SOURCE_DIR}")
    set(FABLE_ZLIB_TARGET fable_zlib)
else()
    find_package(ZLIB QUIET)
    if(ZLIB_FOUND)
        set(FABLE_ZLIB_TARGET ZLIB::ZLIB)
    else()
        include(FetchContent)
        FetchContent_Declare(zlib
            URL https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
            URL_HASH SHA256=9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23
        )
        set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(zlib)
        set(FABLE_ZLIB_TARGET zlibstatic)
        target_include_directories(zlibstatic PUBLIC "${zlib_SOURCE_DIR}" "${zlib_BINARY_DIR}")
    endif()
endif()

# LZO: optional, OFF by default. The QUESTS and FACTIONS text blobs are LZO1X streams from
# lzo1x_999_compress (retail 0xC08130). Upstream LZO 2.10 reproduces every game-written blob
# byte for byte, but LZO is GPL-2.0-or-later, so it is never vendored in this MIT tree: with
# the option ON it is fetched (or taken from FABLE_LZO_SOURCE_DIR) at configure time, and a
# binary built that way is a combined work under the GPL. With it OFF, text blobs still decode
# (our own decoder) and re-encode in retail's stored form, which the retail reader accepts.
option(FABLE_SAVE_WITH_LZO "Link upstream LZO 2.10 (GPL-2.0-or-later) for byte-exact QUESTS/FACTIONS re-encoding" OFF)
set(FABLE_LZO_SOURCE_DIR "" CACHE PATH "LZO 2.10 source tree to build instead of downloading it")

if(FABLE_SAVE_WITH_LZO)
    if(NOT FABLE_LZO_SOURCE_DIR)
        include(FetchContent)
        FetchContent_Declare(lzo
            URL https://www.oberhumer.com/opensource/lzo/download/lzo-2.10.tar.gz
            URL_HASH SHA256=c0f892943208266f9b6543b3ae308fab6284c5c90e627931446fb49b4221a072
            SOURCE_SUBDIR no-cmake-build
        )
        FetchContent_MakeAvailable(lzo)
        set(FABLE_LZO_SOURCE_DIR "${lzo_SOURCE_DIR}")
    endif()
    add_library(fable_lzo STATIC
        "${FABLE_LZO_SOURCE_DIR}/src/lzo1x_9x.c"
        "${FABLE_LZO_SOURCE_DIR}/src/lzo_init.c"
    )
    target_include_directories(fable_lzo PUBLIC "${FABLE_LZO_SOURCE_DIR}/include")
endif()

add_library(fable_save
    src/Entities.cpp
    src/Lzo.cpp
    src/PersistStream.cpp
    src/Profile.cpp
    src/Regions.cpp
    src/SaveBody.cpp
    src/SaveContainer.cpp
    src/SaveHeader.cpp
    src/TextSections.cpp
    src/Zlib.cpp
)

target_include_directories(fable_save
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(fable_save PRIVATE ${FABLE_ZLIB_TARGET})
if(FABLE_SAVE_WITH_LZO)
    target_link_libraries(fable_save PRIVATE fable_lzo)
    target_compile_definitions(fable_save PRIVATE FABLE_SAVE_HAVE_LZO=1)
endif()

target_compile_features(fable_save PUBLIC cxx_std_23)
set_target_properties(fable_save PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(fable_save PRIVATE /W4 /permissive-)
else()
    target_compile_options(fable_save PRIVATE
        -Wall
        -Wextra
        -Wconversion
        -Wpedantic
    )
endif()

add_executable(fable_save_check
    tools/fable_save_check.cpp
)
target_link_libraries(fable_save_check PRIVATE fable_save)

if(BUILD_TESTING)
    add_executable(fable_save_tests
        tests/SaveContainerTests.cpp
    )
    target_link_libraries(fable_save_tests PRIVATE fable_save)

    if(MSVC)
        target_compile_options(fable_save_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(fable_save_tests PRIVATE
            -Wall
            -Wextra
            -Wconversion
            -Wpedantic
        )
    endif()

    add_test(NAME fable_save.SaveContainer
        COMMAND fable_save_tests
    )
endif()
