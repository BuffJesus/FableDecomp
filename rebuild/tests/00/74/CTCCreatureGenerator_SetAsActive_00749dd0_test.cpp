#include <stdio.h>
struct CTCCreatureGenerator {
    char pad00[0x4c]; bool active; char pad4d[0x0f]; unsigned char activationState;
    void SetAsActive(bool value);
};
void CTCCreatureGenerator::SetAsActive(bool value) { active = value; activationState = 0; }
int main() {
    CTCCreatureGenerator object;
    object.active = false; object.activationState = 0xff;
    object.SetAsActive(true);
    if (!object.active || object.activationState != 0) return 1;
    object.activationState = 0xff; object.SetAsActive(false);
    if (object.active || object.activationState != 0) return 1;
    printf("CREATURE_GENERATOR_ACTIVE PASS\n"); return 0;
}