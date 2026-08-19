#pragma optimize("s",on)
// `switch` form of "is this the system event kind": VC7.1 lowers the single case to
// `dec eax; dec eax; je`. __fastcall this=ecx, event=stack (ret 4).
struct Event { int type; };
struct T { bool IsSystemEvent(const Event* e); };
bool T::IsSystemEvent(const Event* e) {
    switch (e->type) {
    case 2:
        return true;
    }
    return false;
}
