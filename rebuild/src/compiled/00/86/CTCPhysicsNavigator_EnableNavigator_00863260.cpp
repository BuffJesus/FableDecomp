struct CTCPhysicsNavigator {
    char pad0[8];
    bool enabled;
    void EnableNavigator(bool enable);
};

void CTCPhysicsNavigator::EnableNavigator(bool enable)
{
    this->enabled = enable;
}