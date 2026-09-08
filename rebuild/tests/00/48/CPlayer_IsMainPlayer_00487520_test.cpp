#include <stdio.h>
struct CPlayerManager { long mainNumber; long GetMainPlayerNumber() const; };
long CPlayerManager::GetMainPlayerNumber() const { return mainNumber; }
struct CPlayer { char a[0x10]; CPlayerManager* manager; char b[0x14]; long number;
 unsigned char IsMainPlayer() const; };
unsigned char CPlayer::IsMainPlayer() const { long playerNumber=number;
 return playerNumber==manager->GetMainPlayerNumber(); }
int main() { CPlayerManager manager; manager.mainNumber=2; CPlayer player;
 player.manager=&manager; player.number=2; if(!player.IsMainPlayer()) return 1;
 player.number=1; if(player.IsMainPlayer()) return 2;
 printf("IS_MAIN_PLAYER PASS\n"); return 0; }