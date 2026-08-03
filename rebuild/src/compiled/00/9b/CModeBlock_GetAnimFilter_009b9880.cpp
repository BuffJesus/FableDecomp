class CAnimFilterBase
{
public:
    int filterData[1];
};

class CModeBlock
{
public:
    CAnimFilterBase* GetAnimFilter();
private:
    void* vfptr;
    void* field4;
    CAnimFilterBase animFilter;
};

CAnimFilterBase* CModeBlock::GetAnimFilter()
{
    return &animFilter;
}