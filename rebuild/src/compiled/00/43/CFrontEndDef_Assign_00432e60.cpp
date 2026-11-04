#include "fable_frontend_def.h"

CFrontEndDef& CFrontEndDef::operator=(const CFrontEndDef& other)
{
    CResource::operator=(other);
    PDefManager = other.PDefManager;
    GlobalIndex = other.GlobalIndex;
    Setup = other.Setup;
    Template = other.Template;
    DefaultValsApplied = other.DefaultValsApplied;
    InstantiationName = other.InstantiationName;
    SubDefInfoMap = other.SubDefInfoMap;
    vAttractModeMovie = other.vAttractModeMovie;
    ErrorMessageBackgroundGraphic = other.ErrorMessageBackgroundGraphic;
    ButtonABigGraphic = other.ButtonABigGraphic;
    ButtonBBigGraphic = other.ButtonBBigGraphic;
    return *this;
}
