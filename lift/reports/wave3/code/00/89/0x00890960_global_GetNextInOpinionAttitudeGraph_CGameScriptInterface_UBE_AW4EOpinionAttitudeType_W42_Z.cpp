#include <cstdint>

class CGameScriptInterface;

enum EOpinionAttitudeType : std::int32_t;

extern EOpinionAttitudeType DAT_01264b00[];

class CGameScriptInterface
{
public:
    EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType attitude) const;
};

EOpinionAttitudeType CGameScriptInterface::GetNextInOpinionAttitudeGraph(
    const EOpinionAttitudeType attitude) const
{
    return DAT_01264b00[attitude];
}