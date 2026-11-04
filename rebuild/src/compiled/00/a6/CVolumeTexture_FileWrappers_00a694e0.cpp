#include "fable_volume_types.h"
#include "fable_file_data_output_stream.h"
#include "fable_data_input_stream.h"
void CVolumeTexture::SaveToFile(CAFile& file)
{
 CFileDataOutputStream stream(&file);
 SaveToDataStream(stream);
}
void CVolumeTexture::LoadFromFile(CAFile& file)
{
 CFileDataInputStream stream(&file,0x4000);
 LoadFromDataStream(stream);
}
