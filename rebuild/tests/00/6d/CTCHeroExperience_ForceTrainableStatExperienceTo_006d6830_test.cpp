#include <stdio.h>

struct CTCHeroExperience {
    char pad[0x18];
    long* stats;
};

void __fastcall ForceTrainableStatExperienceTo(CTCHeroExperience* self, int edxpad, int category, long value);

int main() {
    long arr[8];
    for (int i = 0; i < 8; ++i) arr[i] = -999;
    CTCHeroExperience obj;
    obj.stats = arr;

    ForceTrainableStatExperienceTo(&obj, 0, 3, 500);
    ForceTrainableStatExperienceTo(&obj, 0, 5, -7);   /* negative -> clamp 0 */
    ForceTrainableStatExperienceTo(&obj, 0, 0, 0);

    if (arr[3] == 500 && arr[5] == 0 && arr[0] == 0) {
        printf("OK_006d6830\n");
    } else {
        printf("FAIL %ld %ld %ld\n", arr[3], arr[5], arr[0]);
    }
    return 0;
}