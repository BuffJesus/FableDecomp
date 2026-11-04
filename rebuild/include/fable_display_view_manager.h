#pragma once
#include "fable_counted_pointer.h"
namespace NDisplayView { class CViewBase; }
class CDisplayViewManager {
public:
    CDisplayViewManager();
    CCountedPointer<NDisplayView::CViewBase> PCurrentView;
};
FABLE_STATIC_ASSERT(sizeof(CDisplayViewManager)==8);
