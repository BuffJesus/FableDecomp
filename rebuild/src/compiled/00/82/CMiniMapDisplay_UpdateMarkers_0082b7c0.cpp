struct CMiniMapDisplay {
    char pad[0xbc];
    unsigned char dirtyFlag;
    void RefreshMarkers();
    void UpdateMarkers(bool immediate);
};

void CMiniMapDisplay::UpdateMarkers(bool immediate)
{
    if (immediate)
        RefreshMarkers();
    else
        this->dirtyFlag = 1;
}