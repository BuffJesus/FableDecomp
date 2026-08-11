struct ActionState {
    char pad[0x61];
    char busyFlag;
};

struct CThingCreatureBase {
    char pad[0x100];
    ActionState* action;
    int IsPerformingAnyAction();
};

int CThingCreatureBase::IsPerformingAnyAction()
{
    ActionState* a = this->action;
    if (a != 0) {
        if (a->busyFlag == 0)
            return 1;
    }
    return 0;
}