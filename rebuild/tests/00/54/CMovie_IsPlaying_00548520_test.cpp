#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

int main()
{
    fable_u8 movieBytes[sizeof(CMovieRecoveredTail)];
    memset(movieBytes, 0, sizeof(movieBytes));
    CMovieRecoveredTail& movie =
        *reinterpret_cast<CMovieRecoveredTail*>(movieBytes);

    if (movie.IsPlaying())
        return 1;

    movie.isPlaying15C = 1;
    if (!movie.IsPlaying())
        return 2;

    movie.isPlaying15C = true;
    if (!movie.IsPlaying())
        return 3;

    printf("FABLETLC_CMOVIE_IS_PLAYING PASS\n");
    return 0;
}
