#include "XMLReader.h"
#include "StringDataSource.h"
#include <expat.h>
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include <utility>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> dataSource;
    XML_Parser xmlParser;
    std::queue<SXMLEntity> entityQueue;

    explicit SImplementation(std::shared_ptr<CDataSource> source)
        : dataSource(std::move(source)) {
        xmlParser = XML_ParserCreate(nullptr);
        XML_SetUserData(xmlParser, this);
        XML_SetElementHandler(xmlParser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(xmlParser, CharacterDataHandler);
    }

    ~SImplementation() {
        XML_ParserFree(xmlParser);
    }

    bool IsEnd() const {
        return dataSource->End() && entityQueue.empty();
    }

    bool ReadEntity(SXMLEntity& entity, bool skipCData) {
        RefillEntityQueue();

        while (!entityQueue.empty()) {
            const auto& frontEntity = entityQueue.front();

            if (skipCData && frontEntity.DType == SXMLEntity::EType::CharData) {
                entityQueue.pop();
                continue;
            }

            entity = frontEntity;
            entityQueue.pop();
            return true;
        }
        return false;
    }

private:
    void RefillEntityQueue() {
        if (entityQueue.empty() && !dataSource->End()) {
            std::vector<char> buffer(1024);
            if (dataSource->Read(buffer, buffer.size())) {
                XML_Parse(xmlParser, buffer.data(), buffer.size(), 0);
            } else {
                XML_Parse(xmlParser, nullptr, 0, XML_TRUE); // Signal end of parsing
            }
        }
    }

    void HandleStartElement(const std::string& name, const std::vector<std::string>& attributes) {
        SXMLEntity entity;
        entity.DNameData = name;
        entity.DType = SXMLEntity::EType::StartElement;

        for (size_t i = 0; i < attributes.size(); i += 2) {
            entity.SetAttribute(attributes[i], attributes[i + 1]);
        }

        entityQueue.push(std::move(entity));
    }

    void HandleEndElement(const std::string& name) {
        SXMLEntity entity;
        entity.DNameData = name;
        entity.DType = SXMLEntity::EType::EndElement;
        entityQueue.push(std::move(entity));
    }

    void HandleCharacterData(const std::string& data) {
        if (!entityQueue.empty() && entityQueue.back().DType == SXMLEntity::EType::CharData) {
            entityQueue.back().DNameData += data; // Merge consecutive character data
        } else {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = data;
            entityQueue.push(std::move(entity));
        }
    }

    static void StartElementHandler(void* context, const XML_Char* name, const XML_Char** attributes) {
        auto* impl = static_cast<SImplementation*>(context);
        std::vector<std::string> attrs;

        for (auto attr = attributes; *attr; attr += 2) {
            attrs.emplace_back(attr[0]);
            attrs.emplace_back(attr[1]);
        }

        impl->HandleStartElement(name, attrs);
    }

    static void EndElementHandler(void* context, const XML_Char* name) {
        auto* impl = static_cast<SImplementation*>(context);
        impl->HandleEndElement(name);
    }

    static void CharacterDataHandler(void* context, const XML_Char* data, int length) {
        auto* impl = static_cast<SImplementation*>(context);
        impl->HandleCharacterData(std::string(data, length));
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> source)
    : DImplementation(std::make_unique<SImplementation>(std::move(source))) {}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const {
    return DImplementation->IsEnd();
}

bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipCData) {
    return DImplementation->ReadEntity(entity, skipCData);
}