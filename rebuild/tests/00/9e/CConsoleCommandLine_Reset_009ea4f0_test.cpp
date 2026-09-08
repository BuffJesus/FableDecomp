#include <stdio.h>
#include <string.h>
struct String{const char* Data;String& operator=(const char* s){Data=s;return *this;}};
struct Line{unsigned char base[4];String CurrentEntry;bool UserEntry;unsigned char pad[3];long CursorPosition;void Reset();};
void Line::Reset(){CurrentEntry="";UserEntry=false;CursorPosition=0;}
int main(){Line line;line.CurrentEntry="old";line.UserEntry=true;line.CursorPosition=3;line.Reset();
 if(line.CurrentEntry.Data[0]||line.UserEntry||line.CursorPosition)return 1;printf("CONSOLE_COMMAND_LINE_RESET PASS\n");return 0;}