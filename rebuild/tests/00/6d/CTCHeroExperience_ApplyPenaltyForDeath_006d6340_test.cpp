#include <stdio.h>
struct Experience { unsigned char base[0x10]; long Total,Available; void ApplyPenaltyForDeath(); };
void Experience::ApplyPenaltyForDeath(){const long penalty=Available/2;Available-=penalty;Total-=penalty;}
int main(){Experience e;e.Total=100;e.Available=41;e.ApplyPenaltyForDeath();if(e.Total!=80||e.Available!=21)return 1;
 e.Total=10;e.Available=-3;e.ApplyPenaltyForDeath();if(e.Total!=11||e.Available!=-2)return 2;
 printf("HERO_EXPERIENCE_DEATH_PENALTY PASS\n");return 0;}