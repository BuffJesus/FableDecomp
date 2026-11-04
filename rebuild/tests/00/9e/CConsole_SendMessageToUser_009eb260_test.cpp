#include "test_support/string_vector_fixture.h"
#include "fable_console.h"
static int roots,deathsOfRoots,calls;static CConsole* expectedThis;static const CCharString* expectedText;static bool valid=true;
CBaseClass::CBaseClass(){++roots;}
CBaseClass::~CBaseClass(){++deathsOfRoots;}
void CConsole::AddCommandLineToHistory(const CConsoleCommandLine& line){
 ++calls;valid=valid&&this==expectedThis&&!line.UserEntry&&line.CursorPosition==expectedText->GetLength()&&line.CurrentEntry.GetLength()==expectedText->GetLength()&&memcmp(line.CurrentEntry.GetConstCharPtr(),expectedText->GetConstCharPtr(),line.CurrentEntry.GetLength())==0&&live==2;
}
int main(){
 unsigned long console[sizeof(CConsole)/4];memset(console,0xa5,sizeof(console));expectedThis=(CConsole*)console;
 const char texts[][12]={"", "Hello!", "two\0parts"};const long lengths[]={0,6,9};
 for(int i=0;i<3;++i){CCharString text(texts[i],lengths[i]);expectedText=&text;expectedThis->SendMessageToUser(text);
 if(!valid||roots!=i+1||deathsOfRoots!=i+1||calls!=i+1||live!=1)return 1;}
 for(unsigned j=0;j<sizeof(console)/4;++j)if(console[j]!=0xa5a5a5a5)return 2;
 if(live||dataLive)return 3;puts("SEND_MESSAGE_PASS");return 0;
}