#include "fable_data_input_stream.h"
#include "fable_string.h"
CCharString CDataInputStream::ReadNullTerminatedString(){
 CCharString result;
 char character;
 while((character=ReadValue<char>())!=0)result+=character;
 return result;
}
