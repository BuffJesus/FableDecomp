#include "fable_file_data_output_stream.h"
void CFileDataOutputStream::Write(const void* data, long size) { File->Write(data, size, 0); }
