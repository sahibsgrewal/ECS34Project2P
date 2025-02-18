#include "XMLWriter.h"
#include "StringDataSink.h"
#include <memory>
#include <vector>
#include <string>
#include <utility>

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> sink;

    explicit SImplementation(std::shared_ptr<CDataSink> sink)
        : sink(std::move(sink)) {}

    // Helper function to escape special XML characters
    std::string EscapeSpecialChars(const std::string& input) {
        std::string result;
        for (char ch : input) {
            switch (ch) {
                case '&':  result += "&amp;"; break;
                case '<':  result += "&lt;"; break;
                case '>':  result += "&gt;"; break;
                case '\"': result += "&quot;"; break;
                case '\'': result += "&apos;"; break;
                default:   result += ch; break;
            }
        }
        return result;
    }

    // Write a string to the data sink
    void WriteToSink(const std::string& data) {
        sink->Write(std::vector<char>(data.begin(), data.end()));
    }

    // Write a start element
    void StartElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes) {
        std::string element = "<" + EscapeSpecialChars(name);
        for (const auto& [key, value] : attributes) {
            element += " " + EscapeSpecialChars(key) + "=\"" + EscapeSpecialChars(value) + "\"";
        }
        element += ">";
        WriteToSink(element);
    }

    // Write an end element
    void EndElement(const std::string& name) {
        std::string element = "</" + EscapeSpecialChars(name) + ">";
        WriteToSink(element);
    }

    // Write a complete (self-closing) element
    void CompleteElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes) {
        std::string element = "<" + EscapeSpecialChars(name);
        for (const auto& [key, value] : attributes) {
            element += " " + EscapeSpecialChars(key) + "=\"" + EscapeSpecialChars(value) + "\"";
        }
        element += "/>";
        WriteToSink(element);
    }

    // Write character data
    void CharData(const std::string& data) {
        WriteToSink(EscapeSpecialChars(data));
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink))) {}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush() {
    return true; // No buffer to flush
}

bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
    switch (entity.DType) {
        case SXMLEntity::EType::StartElement:
            DImplementation->StartElement(entity.DNameData, entity.DAttributes);
            break;
        case SXMLEntity::EType::EndElement:
            DImplementation->EndElement(entity.DNameData);
            break;
        case SXMLEntity::EType::CompleteElement:
            DImplementation->CompleteElement(entity.DNameData, entity.DAttributes);
            break;
        case SXMLEntity::EType::CharData:
            DImplementation->CharData(entity.DNameData);
            break;
        default:
            return false; // Unknown entity type
    }
    return true;
}