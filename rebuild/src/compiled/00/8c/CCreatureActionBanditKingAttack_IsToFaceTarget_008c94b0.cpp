#include "rebuild_abi.h"
struct CCreatureAction_BanditKingAttack { char pad[0x114]; unsigned char m_v; char IsToFaceTarget(); };
char CCreatureAction_BanditKingAttack::IsToFaceTarget(){ return (char)m_v; }