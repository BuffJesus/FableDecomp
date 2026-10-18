# Platform services for the fork (MODERNIZATION step 3). First piece: mouse input.
add_library(fable_platform INTERFACE)
target_include_directories(fable_platform INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(fable_platform INTERFACE cxx_std_23)

if(WIN32)
    add_library(fable_platform_win32
        src/RawMouseWin32.cpp
    )
    target_link_libraries(fable_platform_win32 PUBLIC fable_platform PRIVATE user32)
    add_executable(fable_rawmouse_probe
        tools/fable_rawmouse_probe.cpp
    )
    target_link_libraries(fable_rawmouse_probe PRIVATE fable_platform_win32)
endif()

if(BUILD_TESTING)
    add_executable(fable_platform_tests
        tests/MouseAccumulatorTests.cpp
    )
    target_link_libraries(fable_platform_tests PRIVATE fable_platform)
    add_test(NAME fable_platform.MouseAccumulator
        COMMAND fable_platform_tests
    )
    add_executable(fable_worldclock_tests
        tests/WorldClockTests.cpp
    )
    target_link_libraries(fable_worldclock_tests PRIVATE fable_platform)
    add_test(NAME fable_platform.WorldClock
        COMMAND fable_worldclock_tests
    )
    add_executable(fable_keyboard_tests
        tests/KeyboardAccumulatorTests.cpp
    )
    target_link_libraries(fable_keyboard_tests PRIVATE fable_platform)
    add_test(NAME fable_platform.Keyboard
        COMMAND fable_keyboard_tests
    )
endif()
