#include "XMLWriter.h"
#include "StringDataSink.h"
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include "StringUtils.h"

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    
    SImplementation(std::shared_ptr<CDataSink> sink) : Sink(sink) {
    }
    
    void HandleSpecial(std::ostream& os, const std::string& value) {
        for (char ch : value) {
            switch (ch) {
                case '&': os << "&amp;"; break;
                case '<': os << "&lt;"; break;
                case '>': os << "&gt;"; break;
                case '\"': os << "&quot;"; break;
                case '\'': os << "&apos;"; break;
                default: os << ch; break;
            }
        }
    }
    
    void WriteToSink(const std::string& str) {
        Sink->Write(std::vector<char>(str.begin(), str.end()));
    }
    
    void StartElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes) {
        std::stringstream ss;
        ss << "<";
        HandleSpecial(ss, name);
        for (const auto& attr : attributes) {
            ss << " ";
            HandleSpecial(ss, attr.first);
            ss << "=\"";
            HandleSpecial(ss, attr.second);
            ss << "\"";
        }
        ss << ">";
        if (name == "osm") {
            ss << "\n\t\n\t";
        }
        WriteToSink(ss.str());
    }
    
    void EndElement(const std::string& name) {
        std::stringstream ss;
        if (name == "node") {
            ss << "\n\t\n\t";
        } else if (name == "osm") {
            ss << "\n";
        }
        ss << "</";
        HandleSpecial(ss, name);
        ss << ">";
        WriteToSink(ss.str());
    }
    
    void CompleteElement(const std::string& name, const std::vector<SXMLEntity::TAttribute>& attributes) {
        std::stringstream ss;
        ss << "<";
        HandleSpecial(ss, name);
        for (const auto& attr : attributes) {
            ss << " ";
            HandleSpecial(ss, attr.first);
            ss << "=\"";
            HandleSpecial(ss, attr.second);
            ss << "\"";
        }
        ss << "/>";
        if (name == "node") {
            ss << "\n\t\n\t";
        }
        WriteToSink(ss.str());
    }
    
    void CharData(const std::string& data) {
        std::stringstream ss;
        HandleSpecial(ss, data);
        WriteToSink(ss.str());
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(sink)) {
}

CXMLWriter::~CXMLWriter() {
}

bool CXMLWriter::Flush() {
    return true;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
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
            return false;
    }
    return true;
}