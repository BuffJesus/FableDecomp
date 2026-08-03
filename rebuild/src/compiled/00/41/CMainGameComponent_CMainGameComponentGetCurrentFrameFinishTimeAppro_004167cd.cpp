class CMainGameComponent
{
public:
    double GetHelperTimeValue();
    double GetCurrentFrameFinishTimeApproximation();
    char pad[0x58];
    double m_frameFinishOffset;
};

double CMainGameComponent::GetCurrentFrameFinishTimeApproximation()
{
    return GetHelperTimeValue() + m_frameFinishOffset;
}