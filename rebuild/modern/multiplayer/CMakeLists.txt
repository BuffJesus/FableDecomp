add_library(fable_multiplayer
    src/GameEvent.cpp
)

target_include_directories(fable_multiplayer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(fable_multiplayer PUBLIC cxx_std_23)
set_target_properties(fable_multiplayer PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(fable_multiplayer PRIVATE /W4 /permissive-)
else()
    target_compile_options(fable_multiplayer PRIVATE
        -Wall
        -Wextra
        -Wconversion
        -Wpedantic
    )
endif()

if(BUILD_TESTING)
    add_executable(fable_multiplayer_tests
        tests/GameEventTests.cpp
    )
    target_link_libraries(fable_multiplayer_tests PRIVATE fable_multiplayer)

    if(MSVC)
        target_compile_options(fable_multiplayer_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(fable_multiplayer_tests PRIVATE
            -Wall
            -Wextra
            -Wconversion
            -Wpedantic
        )
    endif()

    add_test(NAME fable_multiplayer.GameEvent
        COMMAND fable_multiplayer_tests
    )
endif()
