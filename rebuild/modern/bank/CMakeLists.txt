add_library(fable_bank
    src/BigBank.cpp
)

target_include_directories(fable_bank
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(fable_bank PUBLIC cxx_std_23)
set_target_properties(fable_bank PROPERTIES
    CXX_EXTENSIONS OFF
)

if(MSVC)
    set(FABLE_BANK_WARNINGS /W4 /permissive-)
else()
    set(FABLE_BANK_WARNINGS -Wall -Wextra -Wconversion -Wpedantic)
endif()
target_compile_options(fable_bank PRIVATE ${FABLE_BANK_WARNINGS})

add_executable(fable_bank_check
    tools/fable_bank_check.cpp
)
target_link_libraries(fable_bank_check PRIVATE fable_bank)
target_compile_options(fable_bank_check PRIVATE ${FABLE_BANK_WARNINGS})

if(BUILD_TESTING)
    add_executable(fable_bank_tests
        tests/BigBankTests.cpp
    )
    target_link_libraries(fable_bank_tests PRIVATE fable_bank)
    target_compile_options(fable_bank_tests PRIVATE ${FABLE_BANK_WARNINGS})

    add_test(NAME fable_bank.BigBank
        COMMAND fable_bank_tests
    )
endif()
