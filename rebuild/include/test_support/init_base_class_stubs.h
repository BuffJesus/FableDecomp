#pragma once

// Link-only CBaseClass/CInitBaseClass bodies for fixtures whose leaf constructs or
// destroys a CInitBaseClass-derived object but does not test the base. The
// verified bodies are 0x0099A2D0..0x0099A490; include this in one fixture TU.
#include "fable_init_base_class.h"
#include "test_support/base_class_stubs.h"

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;

CInitBaseClass::CInitBaseClass() : Valid(false) {}
CInitBaseClass::~CInitBaseClass() {}
void CInitBaseClass::Validate() const {}
