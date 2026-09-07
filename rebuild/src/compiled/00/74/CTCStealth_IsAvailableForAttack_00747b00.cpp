struct CTCStealth {
    int GetAttackAvailability(bool includePending);
    bool IsAvailableForAttack();
};
bool CTCStealth::IsAvailableForAttack() {
    return GetAttackAvailability(false) == -1;
}