#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVReader, BasicReadTest) {
    // Prepare a sample string
    std::string dsvString = "Sahib,Grewal,34\nOmar,Taha,43\n";

    // Create a StringDataSource with the sample string
    CStringDataSource source(dsvString);

    // Create a DSVReader
    CDSVReader reader(std::make_shared<CStringDataSource>(source), ',');

    // Test rows
    std::vector<std::string> row1, row2;

    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row1));
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row2));
    EXPECT_TRUE(reader.End());

    // Check the content
    ASSERT_EQ(row1.size(), 3);
    EXPECT_EQ(row1[0], "Sahib");
    EXPECT_EQ(row1[1], "Grewal");
    EXPECT_EQ(row1[2], "34");

    ASSERT_EQ(row2.size(), 3);
    EXPECT_EQ(row2[0], "Omar");
    EXPECT_EQ(row2[1], "Taha");
    EXPECT_EQ(row2[2], "43");
}

TEST(DSVReader, ReadEmptyFieldsTest) {
    // Prepare a sample string with empty fields
    std::string dsvString = "Sahib,,34\n,Grewal,\n";

    // Create a StringDataSource with the sample string
    CStringDataSource source(dsvString);

    // Create a DSVReader
    CDSVReader reader(std::make_shared<CStringDataSource>(source), ',');

    // Test rows with empty fields
    std::vector<std::string> row1, row2;

    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row1));
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row2));
    EXPECT_TRUE(reader.End());

    // Check the content of the rows with empty fields
    ASSERT_EQ(row1.size(), 3);
    EXPECT_EQ(row1[0], "Sahib");
    EXPECT_EQ(row1[1], "");
    EXPECT_EQ(row1[2], "34");

    ASSERT_EQ(row2.size(), 3);
    EXPECT_EQ(row2[0], "");
    EXPECT_EQ(row2[1], "Grewal");
    EXPECT_EQ(row2[2], "");
}
// Test reading and writing with no quotes
TEST(DSVWriter, WriteWithoutQuotesTest) {
    // Create a DSVWriter with StringDataSink and no quotes
    CStringDataSink sink;

    CDSVWriter writer(std::make_shared<CStringDataSink>(sink), ',', false);

    // Write rows without quotes
    writer.WriteRow({"Sahib", "Grewal", "34"});
    writer.WriteRow({"Omar", "Taha", "43"});

    // Check the content of the StringDataSink
    EXPECT_EQ(sink.String(), "Sahib,Grewal,34\nOmar,Taha,43");
}

// Test reading and writing with different line endings
TEST(DSVWriter, WriteWithDifferentLineEndingsTest) {
    // Create a DSVWriter with a StringDataSink and different line endings
    CStringDataSink sink;

    CDSVWriter writer(std::make_shared<CStringDataSink>(sink), ',', true);

    // Write rows with different line endings
    writer.WriteRow({"Sahib", "Grewal", "34"});
    writer.WriteRow({"Omar", "Taha", "43"});

    // Check the content of the StringDataSink
    EXPECT_EQ(sink.String(), "\"Sahib\",\"Grewal\",\"34\"\r\n\"Omar\",\"Taha\",\"43\"\n");
}
// Test reading and writing with escaped characters
TEST(DSVWriter, WriteWithEscapedCharactersTest) {
    // Create a DSVWriter with a StringDataSink and escape characters
    CStringDataSink sink;

    CDSVWriter writer(std::make_shared<CStringDataSink>(sink), ',', true);

    // Write rows with escaped characters
    writer.WriteRow({"Sahib", "Grewal, Student", "34"});
    writer.WriteRow({"Omar", "Taha \"TA\"", "43"});

    // Check the content of the StringDataSink
    EXPECT_EQ(sink.String(), "\"Sahib\",\"Grewal, Student\",\"34\"\n\"Omar\",\"Taha \"\"TA\"\"\",\"43\"");
}
