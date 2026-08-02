#include <cstdio>

class CMainGameComponent
{
public:
    double GetHelperTimeValue();
    double GetCurrentFrameFinishTimeApproximation();
    char pad[0x58];
    double m_frameFinishOffset;
};

double CMainGameComponent::GetHelperTimeValue()
{
    return 10.0;
}

double CMainGameComponent::GetCurrentFrameFinishTimeApproximation()
{
    return GetHelperTimeValue() + m_frameFinishOffset;
}

int main()
{
    CMainGameComponent obj;
    obj.m_frameFinishOffset = 5.5;
    double result = obj.GetCurrentFrameFinishTimeApproximation();
    if (result == 15.5)
    {
        printf("PASS_MARKER_004167CD\n");
    }
    else
    {
        printf("FAIL %f\n", result);
    }
    return 0;
}