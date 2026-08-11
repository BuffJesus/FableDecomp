#include "fable_string.h"

// CEngineScreenEffectDisplacementRenderer is a screen-effect renderer whose
// console-registration virtuals return the fixed script-console function name.
// The retail leaf ignores its 'this' pointer and simply materialises the
// CCharString return slot from a fixed string literal with length -1
// (auto-measure).
class CEngineScreenEffectDisplacementRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectDisplacementRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("EnableScreenEffectDisplacement", -1);
}