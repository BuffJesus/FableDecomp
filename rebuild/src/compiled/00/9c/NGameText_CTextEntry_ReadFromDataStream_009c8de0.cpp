#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
// Provisional names: original text-read context words; category semantics unproved.
extern unsigned long g_NativeTextReadContext_0138DE48;
extern unsigned long g_NativeTextReadContext_0138E064;
class TextReadContext {
 unsigned long first,second;
public:
 TextReadContext():first(g_NativeTextReadContext_0138DE48),second(g_NativeTextReadContext_0138E064){g_NativeTextReadContext_0138DE48=11;g_NativeTextReadContext_0138E064=11;}
 ~TextReadContext(){g_NativeTextReadContext_0138DE48=first;g_NativeTextReadContext_0138E064=second;}
};
void NGameText::CTextEntry::ReadFromDataStream(CDataInputStream& stream)
{
    TextReadContext context;
    Text=stream.ReadNullTerminatedWString();
    SpeechBank=stream.ReadPresizedString();
    Narrator=stream.ReadPresizedString();
    SoundName=stream.ReadPresizedString();
    long count=stream.ReadValue<long>();
    Tags.resize(count);
    for(long i=0;i<count;++i){Tags[i].Pos=stream.ReadValue<long>();Tags[i].Name=stream.ReadNullTerminatedString();}
    SetResourceSize(Sizeof());
}
