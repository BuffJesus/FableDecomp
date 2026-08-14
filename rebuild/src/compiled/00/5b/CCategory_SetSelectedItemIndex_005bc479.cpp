#pragma optimize("s",on)
struct CCategory {
    char pad[0x20];
    long field20;
    long field24;
    void SetSelectedItemIndex(long index);
};

void CCategory::SetSelectedItemIndex(long index)
{
    this->field20 = index;
    this->field24 = index;
    if (index == -1)
        this->field24 = 0;
}