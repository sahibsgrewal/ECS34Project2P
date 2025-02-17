#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include <vector>
#include <iostream>

struct CXMLReader::SImplementation {
    XML_Parser parser;
    std::shared_ptr<CDataSource> dataSource;
    std::queue<SXMLEntity> entityQueue;

    SImplementation(std::shared_ptr<CDataSource> src)
        : parser(XML_ParserCreate(nullptr)), dataSource(std::move(src)) {
        InitializeParser();
    }

    ~SImplementation() {
        XML_ParserFree(parser);
    }

    bool End() const {
        return dataSource->End();
    }

    bool ReadEntity(SXMLEntity& entity, bool skipCData) {
        ProcessDataBuffer();

        if (entityQueue.empty()) return false;

        if (skipCData) {
            DiscardCharacterData();
        }

        if (!entityQueue.empty()) {
            entity = entityQueue.front();
            entityQueue.pop();
            return true;
        }

        return false;
    }

private:
    void InitializeParser() {
        XML_SetUserData(parser, this);
        XML_SetStartElementHandler(parser, &SImplementation::StartElementHandler);
        XML_SetEndElementHandler(parser, &SImplementation::EndElementHandler);
        XML_SetCharacterDataHandler(parser, &SImplementation::CharacterDataHandler);
    }

    void ProcessDataBuffer() {
        std::vector<char> dataBuffer(256);
        
        while (entityQueue.empty() && !dataSource->End()) {
            const auto bytesRead = dataSource->Read(dataBuffer, dataBuffer.size());
            XML_Parse(parser, dataBuffer.data(), bytesRead, dataSource->End());
        }
    }

    void DiscardCharacterData() {
        while (!entityQueue.empty() && entityQueue.front().DType == SXMLEntity::EType::CharData) {
            entityQueue.pop();
        }
    }

    static void StartElementHandler(void* context, const XML_Char* name, const XML_Char** attributes) {
        auto* impl = static_cast<SImplementation*>(context);
        impl->HandleStartElement(name, attributes);
    }

    static void EndElementHandler(void* context, const XML_Char* name) {
        auto* impl = static_cast<SImplementation*>(context);
        impl->HandleEndElement(name);
    }

    static void CharacterDataHandler(void* context, const XML_Char* data, int length) {
        auto* impl = static_cast<SImplementation*>(context);
        impl->HandleCharacterData(std::string(data, length));
    }

    void HandleStartElement(const XML_Char* name, const XML_Char** attributes) {
        SXMLEntity entity;
        entity.DNameData = name;
        entity.DType = SXMLEntity::EType::StartElement;
        AddAttributesToEntity(entity, attributes);
        entityQueue.push(entity);
    }

    void HandleEndElement(const XML_Char* name) {
        SXMLEntity entity;
        entity.DNameData = name;
        entity.DType = SXMLEntity::EType::EndElement;
        entityQueue.push(entity);
    }

    void HandleCharacterData(const std::string& data) {
        if (!data.empty()) {
            SXMLEntity entity;
            entity.DNameData = data;
            entity.DType = SXMLEntity::EType::CharData;
            entityQueue.push(entity);
        }
    }

    void AddAttributesToEntity(SXMLEntity& entity, const XML_Char** attributes) {
        for (size_t i = 0; attributes[i]; i += 2) {
            entity.SetAttribute(attributes[i], attributes[i + 1]);
        }
    }
};

// Constructor and Destructor Implementation
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> source)
    : DImplementation(std::make_unique<SImplementation>(std::move(source))) {}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const {
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipCData) {
    return DImplementation->ReadEntity(entity, skipCData);
}
