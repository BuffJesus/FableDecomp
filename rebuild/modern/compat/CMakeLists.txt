# Header-only types that keep retail (VC7.1) layouts under current compilers.
add_library(fable_compat INTERFACE)
target_include_directories(fable_compat INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(fable_compat INTERFACE cxx_std_17)

if(BUILD_TESTING)
    add_executable(fable_compat_tests
        tests/RetailBitsetTests.cpp
    )
    target_link_libraries(fable_compat_tests PRIVATE fable_compat)
    add_test(NAME fable_compat.RetailBitset
        COMMAND fable_compat_tests
    )
endif()
