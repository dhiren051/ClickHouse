#include <string>
#include <Processors/Formats/IRowOutputFormat.h>
#include <IO/WriteHelpers.h>


namespace DB
{

void IRowOutputFormat::consume(DB::Chunk chunk)
{
    auto num_rows = chunk.getNumRows();
    auto & columns = chunk.getColumns();

    for (UInt64 row = 0; row < num_rows; ++row)
    {
        if (!first_row)
            writeRowBetweenDelimiter();
        first_row = false;

        write(columns, row);
    }
}

void IRowOutputFormat::consumeTotals(DB::Chunk chunk)
{
    auto num_rows = chunk.getNumRows();
    if (num_rows != 1)
        throw Exception("Got " + toString(num_rows) + " in totals chunk, expected 1", ErrorCodes::LOGICAL_ERROR);

    auto & columns = chunk.getColumns();

    writeBeforeTotals();
    write(columns, 0);
    writeAfterTotals();
}

void IRowOutputFormat::consumeExtremes(DB::Chunk chunk)
{
    auto num_rows = chunk.getNumRows();
    auto & columns = chunk.getColumns();

    writeBeforeExtremes();

    for (UInt64 row = 0; row < num_rows; ++row)
    {
        if (row != 0)
            writeRowBetweenDelimiter();
        first_row = false;

        write(columns, row);
    }

    writeAfterExtremes();
}

void IRowOutputFormat::write(const Columns & columns, size_t row_num)
{
    size_t num_columns = columns.size();

    writeRowStartDelimiter();

    for (size_t i = 0; i < num_columns; ++i)
    {
        if (i != 0)
            writeFieldDelimiter();

        writeField(*columns[i], *types[i], row_num);
    }

    writeRowEndDelimiter();
}

}



