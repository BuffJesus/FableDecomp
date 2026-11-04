#pragma once

// Link-only CBaseClass bodies for fixtures that stand in for CBaseClassNonCopyable
// (or construct a CBaseClass-derived object) without testing the root class. The
// verified bodies are 0x0099A2D0 / 0x0099A2E0; include this in one fixture TU.
#include "fable_base_class.h"

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
