//=== _Cons_val<std::allocator<CQuestManager::CActivateQuestInfo>,CQuestManager::CActivateQuestInfo,CQuestManager::CActivateQuestInfo_const&> @ 004b8130 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CQuestManager::CActivateQuestInfo>,class CQuestManager::CActivateQuestInfo,class
   CQuestManager::CActivateQuestInfo const &>(class std::allocator<class
   CQuestManager::CActivateQuestInfo> &,class CQuestManager::CActivateQuestInfo *,class
   CQuestManager::CActivateQuestInfo const &) */

void __fastcall
std::
_Cons_val<std::allocator<CQuestManager::CActivateQuestInfo>,CQuestManager::CActivateQuestInfo,CQuestManager::CActivateQuestInfo_const&>
          (allocator<CQuestManager::CActivateQuestInfo> *param_1,CActivateQuestInfo *param_2,
          CActivateQuestInfo *param_3)

{
  if (param_1 != (allocator<CQuestManager::CActivateQuestInfo> *)0x0) {
    CCharString::CCharString((CCharString *)param_1,(CCharString *)param_2);
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
    param_1[8] = *(allocator<CQuestManager::CActivateQuestInfo> *)(param_2 + 8);
    param_1[9] = *(allocator<CQuestManager::CActivateQuestInfo> *)(param_2 + 9);
  }
  return;
}



//=== _Cons_val<std::allocator<CQuestManager::CQuestCard>,CQuestManager::CQuestCard,CQuestManager::CQuestCard_const&> @ 004b82f0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class CQuestManager::CQuestCard>,class
   CQuestManager::CQuestCard,class CQuestManager::CQuestCard const &>(class std::allocator<class
   CQuestManager::CQuestCard> &,class CQuestManager::CQuestCard *,class CQuestManager::CQuestCard
   const &) */

void __fastcall
std::
_Cons_val<std::allocator<CQuestManager::CQuestCard>,CQuestManager::CQuestCard,CQuestManager::CQuestCard_const&>
          (allocator<CQuestManager::CQuestCard> *param_1,CQuestCard *param_2,CQuestCard *param_3)

{
  if (param_1 != (allocator<CQuestManager::CQuestCard> *)0x0) {
    CCharString::CCharString((CCharString *)param_1,(CCharString *)param_2);
    CCharString::CCharString((CCharString *)(param_1 + 4),(CCharString *)(param_2 + 4));
    param_1[8] = *(allocator<CQuestManager::CQuestCard> *)(param_2 + 8);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_2 + 0xc);
  }
  return;
}



