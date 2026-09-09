#include <stdio.h>
#include <string.h>
#include "engine/CEngineWeatherRenderer.h"

int main()
{
    CEngineWeatherRenderer renderer;
    CWeatherMask masks[4];
    int index;

    memset(&renderer, 0, sizeof(renderer));
    memset(masks, 0, sizeof(masks));

    for (index = 0; index < 4; ++index) {
        masks[index].Scale = (float)(index + 1);
        masks[index].BSphere[0] = (unsigned char)(0x10 + index);
        renderer.AddWeatherMask(masks[index], 0xdeadbeef);
    }

    if (renderer.CurrentMask != 1)
        return 1;
    if (renderer.WeatherMaskList[0].Scale != 4.0f ||
        renderer.WeatherMaskList[1].Scale != 2.0f ||
        renderer.WeatherMaskList[2].Scale != 3.0f)
        return 2;
    if (renderer.WeatherMaskList[0].BSphere[0] != 0x13)
        return 3;

    printf("WEATHER_MASK_RING PASS\n");
    return 0;
}
