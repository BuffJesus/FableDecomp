using System.Text;
using SilverChest.Formats.Bin;

if (args.Length < 3)
{
    Console.Error.WriteLine("Usage: BinDump <names.bin> <binFile> <output.csv> [hexFilterDefinition]");
    Environment.Exit(2);
}

string namesPath = args[0];
string binPath = args[1];
string outputPath = args[2];
string? hexFilter = args.Length > 3 ? args[3] : null;

NamesBinFile names = NamesBinFile.Load(namesPath);
BinFile bin = BinFile.Load(binPath, names);

if (hexFilter != null)
{
    // Hex-dump payloads of every entry whose definition matches the filter.
    using StreamWriter hexWriter = new StreamWriter(outputPath, false, new UTF8Encoding(false));
    hexWriter.WriteLine("index\tdefinition\tname\tlength\thex");
    for (int i = 0; i < bin.Entries.Count; i++)
    {
        BinEntry entry = bin.Entries[i];
        if (!string.Equals(entry.Definition, hexFilter, StringComparison.OrdinalIgnoreCase))
        {
            continue;
        }
        hexWriter.WriteLine(string.Join("\t",
            i, entry.Definition, entry.Name, entry.Length,
            Convert.ToHexString(entry.Data)));
    }
    Console.WriteLine($"hex dump done: {outputPath}");
    return;
}

Directory.CreateDirectory(Path.GetDirectoryName(Path.GetFullPath(outputPath))!);
using StreamWriter writer = new StreamWriter(outputPath, false, new UTF8Encoding(false));
writer.WriteLine("index,definition,name,name_crc,length,ascii_preview");

for (int i = 0; i < bin.Entries.Count; i++)
{
    BinEntry entry = bin.Entries[i];
    writer.WriteLine(string.Join(",", new[]
    {
        i.ToString(),
        Csv(entry.Definition),
        Csv(entry.Name),
        $"0x{entry.NameCrc:X8}",
        entry.Length.ToString(),
        Csv(entry.PreviewText(160))
    }));
}

Console.WriteLine($"entries={bin.Entries.Count} output={outputPath}");

static string Csv(string value)
{
    return "\"" + (value ?? string.Empty).Replace("\"", "\"\"") + "\"";
}
