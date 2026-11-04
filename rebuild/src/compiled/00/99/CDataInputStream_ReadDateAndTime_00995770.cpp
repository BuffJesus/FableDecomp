#include "fable_data_input_stream.h"
#include "fable_date_and_time.h"
CDateAndTime CDataInputStream::ReadDateAndTime() {
    long year=ReadValue<long>();
    unsigned long month=ReadValue<unsigned char>();
    unsigned long day=ReadValue<unsigned char>();
    unsigned long hour=ReadValue<unsigned char>();
    unsigned long minute=ReadValue<unsigned char>();
    unsigned long second=ReadValue<unsigned short>();
    unsigned long millisecond=ReadValue<unsigned short>();
    return CDateAndTime(year,month,day,hour,minute,second,millisecond);
}
