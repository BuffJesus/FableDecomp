#include <stdio.h>

#include "fable_movie.h"

bool g_FableMovieLeftAlignText_013C5A7C = false;

int main()
{
    CXMVPlayer::SetLeftAlignText(true);
    if (!g_FableMovieLeftAlignText_013C5A7C)
    {
        puts("FABLETLC_MOVIE_LEFT_ALIGN_BEHAVIOR FAIL code=1");
        return 1;
    }

    CXMVPlayer::SetLeftAlignText(false);
    if (g_FableMovieLeftAlignText_013C5A7C)
    {
        puts("FABLETLC_MOVIE_LEFT_ALIGN_BEHAVIOR FAIL code=2");
        return 2;
    }

    puts("FABLETLC_MOVIE_LEFT_ALIGN_BEHAVIOR PASS");
    return 0;
}
