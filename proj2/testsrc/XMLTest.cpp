
#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

// Test reading basic XML entities
TEST(XMLReader, BasicReadTest) {
    // Create a sample XML string
    std::string xmlString = "<root><element1>Value1</element1><element2>Value2</element2></root>";

    // Create a StringDataSource with sample XML string
    CStringDataSource source(xmlString);

    // Create a XMLReader
    CXMLReader reader(std::make_shared<CStringDataSource>(source));

    // Test reading entities
    SXMLEntity entity1, entity2;

    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadEntity(entity1, false));
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadEntity(entity2, false));
    EXPECT_TRUE(reader.End());

    // Check the content of the entities
    ASSERT_EQ(entity1.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity1.DNameData, "root");
    EXPECT_EQ(entity1.DAttributes.size(), 0);

    ASSERT_EQ(entity2.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity2.DNameData, "element1");
    EXPECT_EQ(entity2.DAttributes.size(), 0);
}

// Test reading entities with attributes
TEST(XMLReader, ReadWithAttributesTest) {
    // Create a sample XML string with attributes
    std::string xmlString = "<person name=\"Sahib\" age=\"34\" />";

    // Create a StringDataSource with the sample XML string
    CStringDataSource source(xmlString);

    // Create a XMLReader
    CXMLReader reader(std::make_shared<CStringDataSource>(source));

    // Test reading entities with attributes
    SXMLEntity entity;

    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_TRUE(reader.End());

    // Check the content of the entity with attributes
    ASSERT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "person");
    ASSERT_EQ(entity.DAttributes.size(), 2);
    EXPECT_EQ(entity.AttributeValue("name"), "Sahib");
    EXPECT_EQ(entity.AttributeValue("age"), "34");
}

// Test writing basic XML entities
TEST(XMLWriter, BasicWriteTest) {
    // Create a StringDataSink
    CStringDataSink sink;

    // Create a XMLWriter with the StringDataSink
    CXMLWriter writer(std::make_shared<CStringDataSink>(sink));

    // Create sample entities
    SXMLEntity entity1, entity2;
    entity1.DType = SXMLEntity::EType::StartElement;
    entity1.DNameData = "root";

    entity2.DType = SXMLEntity::EType::StartElement;
    entity2.DNameData = "element1";

    // Write entities
    writer.WriteEntity(entity1);
    writer.WriteEntity(entity2);

    // Check the content of the StringDataSink
    std::string expectedXmlString = "<root></root><element1></element1>";
    EXPECT_EQ(sink.String(), expectedXmlString);
}

// Test writing entities with attributes
TEST(XMLWriter, WriteWithAttributesTest) {
    // Create a StringDataSink
    CStringDataSink sink;

    // Create a XMLWriter with the StringDataSink
    CXMLWriter writer(std::make_shared<CStringDataSink>(sink));

    // Create an entity with attributes
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "person";
    entity.SetAttribute("name", "Sahib");
    entity.SetAttribute("age", "34");

    // Write the entity
    writer.WriteEntity(entity);

    // Check the content of the StringDataSink
    std::string expectedXmlString = "<person name=\"Sahib\" age=\"34\"></person>";
    EXPECT_EQ(sink.String(), expectedXmlString);
}