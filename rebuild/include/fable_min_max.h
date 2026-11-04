#pragma once

// The engine's reference-returning min/max templates (FableWin
// ??$Maximum@N@@YAABNABN0@Z, retail 0x00A56BB0 out of line for double).
// Inlined, they leave the pointer-select shape retail shows, e.g.
// CAFile::CopyFileW 0x00998E70: `lea eax,[bufferSize]; cmp; jg; lea eax,[remaining]; mov esi,[eax]`.
#ifndef FABLE_MIN_MAX_DEFINED
#define FABLE_MIN_MAX_DEFINED

template <class T>
inline const T& Minimum(const T& a, const T& b)
{
    return a > b ? b : a;
}

template <class T>
inline const T& Maximum(const T& a, const T& b)
{
    return a > b ? a : b;
}

#endif
