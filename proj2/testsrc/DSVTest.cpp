#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVWriter, EmptyRow) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row;
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\n");
}

TEST(DSVWriter, SimpleRow) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row = {"a", "b", "c"};
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "a,b,c\n");
}

TEST(DSVWriter, RowWithDelimiter) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row = {"a,b", "c"};
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a,b\",c\n");
}

TEST(DSVWriter, RowWithQuote) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row = {"a\"b", "c"};
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a\"\"b\",c\n");
}

TEST(DSVWriter, RowWithNewline) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row = {"a\nb", "c"};
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a\nb\",c\n");
}

TEST(DSVWriter, QuoteAllMode) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true);
    std::vector<std::string> row = {"a", "b", "c"};
    
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"a\",\"b\",\"c\"\n");
}
TEST(DSVReader, EmptySource) {
    auto source = std::make_shared<CStringDataSource>("");
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row));
}
TEST(DSVReader, SimpleRow) {
    auto source = std::make_shared<CStringDataSource>("a,b,c\n");
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "c");
    EXPECT_TRUE(reader.End());
}

TEST(DSVReader, QuotedValue) {
    auto source = std::make_shared<CStringDataSource>("\"a,b\",c\n");
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 2);
    EXPECT_EQ(row[0], "a,b");
    EXPECT_EQ(row[1], "c");
    EXPECT_TRUE(reader.End());
}
TEST(DSVReader, EscapedQuotes) {
    auto source = std::make_shared<CStringDataSource>("\"a\"\"b\",c\n");
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 2);
    EXPECT_EQ(row[0], "a\"b");
    EXPECT_EQ(row[1], "c");
    EXPECT_TRUE(reader.End());
}

TEST(DSVReader, MultipleRows) {
    auto source = std::make_shared<CStringDataSource>("a,b\nc,d\n");
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 2);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 2);
    EXPECT_EQ(row[0], "c");
    EXPECT_EQ(row[1], "d");
    
    EXPECT_TRUE(reader.End());
}
