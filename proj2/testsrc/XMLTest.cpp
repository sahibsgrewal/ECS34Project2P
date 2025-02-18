
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

#include <gtest/gtest.h>
#include "XMLWriter.h"
#include "StringDataSink.h"
#include <memory>
#include <string>

TEST(XMLWriterTest, WriteStartElement) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "root";
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "<root>");
}

TEST(XMLWriterTest, WriteEndElement) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = "root";
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "</root>");
}

TEST(XMLWriterTest, WriteCompleteElement) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CompleteElement;
    entity.DNameData = "empty";
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "<empty/>");
}

TEST(XMLWriterTest, WriteCharData) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData = "Hello, World!";
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "Hello, World!");
}

TEST(XMLWriterTest, WriteElementWithAttributes) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "person";
    entity.DAttributes.push_back({"name", "Sahib"});
    entity.DAttributes.push_back({"age", "34"});
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "<person name=\"Sahib\" age=\"34\">");
}

TEST(XMLWriterTest, EscapeSpecialCharacters) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CharData;
    entity.DNameData = "A & B < C > D \" E ' F";
    
    EXPECT_TRUE(Writer->WriteEntity(entity));
    EXPECT_EQ(Sink->String(), "A &amp; B &lt; C &gt; D &quot; E &apos; F");
}

TEST(XMLWriterTest, CompleteDocument) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    std::vector<SXMLEntity> entities = {
        {SXMLEntity::EType::StartElement, "root", {}},
        {SXMLEntity::EType::StartElement, "child", {{"id", "1"}}},
        {SXMLEntity::EType::CharData, "content", {}},
        {SXMLEntity::EType::EndElement, "child", {}},
        {SXMLEntity::EType::EndElement, "root", {}}
    };
    
    for (const auto& entity : entities) {
        EXPECT_TRUE(Writer->WriteEntity(entity));
    }
    
    EXPECT_EQ(Sink->String(), "<root><child id=\"1\">content</child></root>");
}

TEST(XMLWriterTest, InvalidEntityType) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);

    SXMLEntity entity;
    entity.DType = static_cast<SXMLEntity::EType>(999); // Invalid type
    entity.DNameData = "test";
    
    EXPECT_FALSE(Writer->WriteEntity(entity));
}

TEST(XMLWriterTest, FlushOperation) {
    auto Sink = std::make_shared<CStringDataSink>();
    auto Writer = std::make_unique<CXMLWriter>(Sink);
    
    EXPECT_TRUE(Writer->Flush());
}