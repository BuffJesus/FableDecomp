#include "fable_system_manager.h"
CProfile::CProfile(const CProfile& other)
 : Manager(other.Manager), StartTime(other.StartTime), TotalTime(other.TotalTime),
 LastTime(other.LastTime), Running(other.Running), Percentage(other.Percentage),
 Name(other.Name), TreeDepth(other.TreeDepth), NewNode(other.NewNode),
 Visible(other.Visible), CallCounter(other.CallCounter), UpdatedCallCounter(other.UpdatedCallCounter) {}