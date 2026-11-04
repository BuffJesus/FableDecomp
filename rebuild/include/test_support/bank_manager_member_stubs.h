#pragma once
// Empty-bank fixtures only: release the sentinel, without providing any
// bank-loading implementation or the constructors being tested.
#include "fable_system_manager.h"
namespace _STL {
template<> inline list<CGraphicBankManager::CGraphicDataBankInfo>::~list(){free(_M_node);}
template<> inline list<CMeshBankManager::CMeshDataBankInfo>::~list(){free(_M_node);}
}
CGraphicBankManager::~CGraphicBankManager(){}
CMeshBankManager::~CMeshBankManager(){}
