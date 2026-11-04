#include "fable_string_parser.h"
const CCharString& CParsedItem::GetSymbol() const { return String; }
const CCharString& CParsedItem::GetString() const { return String; }
long CParsedItem::GetInteger() const { return Value.Integer; }
float CParsedItem::GetFloat() const { return Value.Float; }
