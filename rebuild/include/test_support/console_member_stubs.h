#pragma once
// Dependencies for fixtures that replace console construction and methods.
// The console constructor under test has a separate candidate-linked fixture.
#include "fable_console.h"
#include "test_support/base_class_stubs.h"
#include "test_support/string_vector_fixture.h"
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
CConsole::~CConsole() {
    free(KeyBindingMap._M_t._M_header._M_data);
    free(StringBindingMap._M_t._M_header._M_data);
    free(RegisteredConsoleInputs._M_t._M_header._M_data);
}
