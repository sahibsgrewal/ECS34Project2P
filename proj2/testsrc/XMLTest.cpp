#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSink.h"
#include "StringDataSource.h"
#include <memory>
#include <string>

TEST(XMLReaderTest, EmptyDocument) {
    auto source = std::make_shared<CStringDataSource>("");
    auto Reader = std::make_unique<CXMLReader>(source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader->End());
    EXPECT_FALSE(Reader->ReadEntity(Entity, false));
}

TEST(XMLReaderTest, SimpleElement) {
    auto source = std::make_shared<CStringDataSource>("<root></root>");
    auto Reader = std::make_unique<CXMLReader>(source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "root");
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "root");
    
    EXPECT_TRUE(Reader->End());
}

TEST(XMLReaderTest, ElementWithAttributes) {
    auto source = std::make_shared<CStringDataSource>("<person name=\"Sahib\" age=\"34\"></person>");
    auto Reader = std::make_unique<CXMLReader>(source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "person");
    EXPECT_EQ(Entity.DAttributes.size(), 2);
    EXPECT_EQ(Entity.DAttributes[0].first, "name");
    EXPECT_EQ(Entity.DAttributes[0].second, "Sahib");
    EXPECT_EQ(Entity.DAttributes[1].first, "34");
    EXPECT_EQ(Entity.DAttributes[1].second, "30");
}

TEST(XMLReaderTest, ElementWithCharacterData) {
    auto source = std::make_shared<CStringDataSource>("<root>Hello, World!</root>");
    auto Reader = std::make_unique<CXMLReader>(source);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "Hello, World!");
    
    EXPECT_TRUE(Reader->ReadEntity(Entity, false));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
}

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