#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include "fable_def_class_base.h"

void CDefinitionManager::CompileDefinition(CStringParser& parser, const CCharString& className, const CCharString& instanceName)
{
    long globalIndex;
    if (!PDefinitionSymbolMap->FindSymbol(instanceName, globalIndex))
    {
        Error(parser, "Unable to find def class called " + instanceName);
        return;
    }
    LoadDef(globalIndex);
    CDefPointer<CDefClassBase> target = InstantiatedDefs[globalIndex]->PDef;
    unsigned long baseLine = parser.Line;
    bool specialised = false;
    CCharString unused;
    CParsedItem next;
    if (parser.PeekNextItem(next) && next.Type == PARSED_STRING && next.GetString() == "specialises")
    {
        parser.ReadAsString();
        CCharString parentName = parser.ReadAsString();
        CDefPointer<CDefClassBase> parent = GetPDefFromInstantiationName(parentName);
        if (parent.Get())
        {
            if (parent->Setup)
            {
                bool wasTemplate = target->Template;
                specialised = true;
                CCharString oldName = static_cast<CCharString>(target->GetInstantiationName());
                target->Copy(parent.Get());
                target->Setup = false;
                target->Template = wasTemplate;
                target->SetInstantiationName(CDefString(oldName));
            }
            else
                Error(parser, CCharString("Def ") + parentName + " is not setup to specialise.");
        }
        else
            Error(parser, CCharString("Def ") + parentName + " does not exist.");
    }
    CCharString body = parser.ReadAsStringUntilString("#end_definition");
    CStringParser bodyParser(body, ErrorCallback, "", baseLine);
    bool useDefaults = !specialised && !target->DefaultValsApplied;
    while (bodyParser.SkipPastString("<"))
    {
        long begin = bodyParser.StringPos - 1;
        CCharString name = bodyParser.ReadAsStringUntilString(">");
        bodyParser.SkipPastNextItemIfIs(">");
        CCharString end = "<\\" + name + ">";
        CCharString subBody = bodyParser.ReadAsStringUntilString(end);
        bodyParser.SkipPastString(end);
        for (long i = begin; i < static_cast<long>(bodyParser.StringPos); ++i)
            if (body.GetConstCharPtr()[i] != '\n') body.SetChar(i, ' ');
        CompileSubDefinition(subBody, name, globalIndex);
    }
    bodyParser.TempCharString = body;
    bodyParser.Init(body.GetConstCharPtr(), ErrorCallback, "", baseLine);
    CPersistContext context(bodyParser, ErrorCallback, useDefaults, this, PSymbolMap.Get(), &body);
    target->Transfer(context);
    target->Setup = true;
    target->DefaultValsApplied = true;
    context.CheckUnknownSymbols();
}
