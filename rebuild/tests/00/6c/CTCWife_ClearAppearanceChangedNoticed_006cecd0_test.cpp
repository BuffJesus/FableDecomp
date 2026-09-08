#include <stdio.h>
enum EWifeAppearanceCategories { WIFE_HAIR=0, WIFE_CLOTHING=1 };
struct CTCWifeRetail { char pad[0xA4]; long frames[16];
 void ClearAppearanceChangedNoticed(const EWifeAppearanceCategories&); };
void CTCWifeRetail::ClearAppearanceChangedNoticed(const EWifeAppearanceCategories& category) {
 frames[category]=0; }
int main() { CTCWifeRetail wife; wife.frames[0]=20; wife.frames[1]=40;
 EWifeAppearanceCategories category=WIFE_CLOTHING; wife.ClearAppearanceChangedNoticed(category);
 if(wife.frames[0]!=20 || wife.frames[1]!=0) return 1;
 printf("WIFE_APPEARANCE_NOTICE_CLEAR PASS\n"); return 0; }