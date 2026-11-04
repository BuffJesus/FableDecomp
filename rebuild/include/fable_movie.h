#pragma once

#include "rebuild_abi.h"

class CXMVPlayer
{
public:
    static void FABLE_FASTCALL SetLeftAlignText(bool enabled);
    // ?StaticReleaseXMVCode@CXMVPlayer@@SIXXZ, retail 0x009BCFC0 (caller
    // CNewFrontendGameComponent::ReleaseXMVCode 0x0042DBD8).
    static void FABLE_FASTCALL StaticReleaseXMVCode();
};

extern bool g_FableMovieLeftAlignText_013C5A7C;
