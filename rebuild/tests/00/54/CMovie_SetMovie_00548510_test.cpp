#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

namespace
{
    CWideStringData g_sourceStorage;
    CWideStringData g_destinationStorage;
    unsigned int g_assignmentCalls;

    CWideStringData*& Storage(CWideString& text)
    {
        return *reinterpret_cast<CWideStringData**>(&text);
    }
}

CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assignmentCalls;
    Storage(*this) = Storage(const_cast<CWideString&>(other));
    return *this;
}

int main()
{
    fable_u8 movieBytes[sizeof(CMovieRecoveredTail)];
    fable_u8 sourceBytes[sizeof(CWideString)];
    memset(movieBytes, 0, sizeof(movieBytes));
    memset(sourceBytes, 0, sizeof(sourceBytes));
    CMovieRecoveredTail& movie =
        *reinterpret_cast<CMovieRecoveredTail*>(movieBytes);
    CWideString& source =
        *reinterpret_cast<CWideString*>(sourceBytes);

    g_sourceStorage.text =
        const_cast<wchar_t*>(L"intro_comp.wmv");
    g_destinationStorage.text =
        const_cast<wchar_t*>(L"old_movie.wmv");
    Storage(source) = &g_sourceStorage;
    Storage(movie.movieName174) = &g_destinationStorage;

    movie.SetMovie(source);
    if (
        Storage(movie.movieName174) != &g_sourceStorage ||
        g_assignmentCalls != 1)
    {
        return 1;
    }

    printf("FABLETLC_CMOVIE_SET_MOVIE PASS\n");
    return 0;
}
