#include "XMLWriter.h"
#include <sstream>

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;

    SImplementation(std::shared_ptr<CDataSink> sink) : Sink(sink) {}

    // Converts special XML characters into escaped equivalents
    std::string EscapeString(const std::string& input) {
        std::ostringstream result;
        for (char c : input) {
            switch (c) {
                case '&': result << "&amp;"; break;
                case '<': result << "&lt;"; break;
                case '>': result << "&gt;"; break;
                case '"': result << "&quot;"; break;
                case '\'': result << "&apos;"; break;
                default: result << c;
            }
        }
        return result.str();
    }

    // Writes a string to data sink
    void WriteToDisk(const std::string& str) {
        Sink->Write(std::vector<char>(str.begin(), str.end()));
    }

    // Formats attributes into format
    std::string FormatAttributes(const std::vector<SXMLEntity::TAttribute>& attrs) {
        std::ostringstream result;
        for (const auto& attr : attrs) {
            result << " " << EscapeString(attr.first) << "=\"" << EscapeString(attr.second) << "\"";
        }
        return result.str();
    }

    // Handles writing XML entities
    void HandleEntity(const SXMLEntity& entity) {
        std::ostringstream output;

        if (entity.DType == SXMLEntity::EType::StartElement) {
            // Opening tag with attributes
            output << "<" << EscapeString(entity.DNameData) << FormatAttributes(entity.DAttributes) << "></" 
                   << EscapeString(entity.DNameData) << ">";
        } 
        else if (entity.DType == SXMLEntity::EType::EndElement) {
            // Closing tag
            output << "</" << EscapeString(entity.DNameData) << ">";
        } 
        else if (entity.DType == SXMLEntity::EType::CompleteElement) {
            // Self-closing tag
            output << "<" << EscapeString(entity.DNameData) << FormatAttributes(entity.DAttributes) << "/>";
        } 
        else if (entity.DType == SXMLEntity::EType::CharData) {
            // Text content inside element
            output << EscapeString(entity.DNameData);
        }

        WriteToDisk(output.str());
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(sink)) {}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush() {
    return true;
}

bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
    if (entity.DType >= SXMLEntity::EType::StartElement && entity.DType <= SXMLEntity::EType::CharData) {
        DImplementation->HandleEntity(entity);
        return true;
    }
    return false;
}