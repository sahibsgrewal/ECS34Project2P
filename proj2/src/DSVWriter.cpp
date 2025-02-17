#include "DSVWriter.h"
#include <memory>
#include "StringDataSink.h"
#include <vector>
#include <string>
#include <sstream>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    char Delimiter;
    bool QuoteAll;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) 
        : Sink(sink), Delimiter(delimiter), QuoteAll(quoteall) {}

    SImplementation()
        : Sink(nullptr), Delimiter(','), QuoteAll(false) {}
};
CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(sink, delimiter, quoteall)) {
}

CDSVWriter::~CDSVWriter() { // destructor
}

bool CDSVWriter::WriteRow(const std::vector<std::string>& dataRow) {
    std::vector<std::string> formattedFields;
    formattedFields.reserve(dataRow.size());

    for (const auto& fieldValue : dataRow) {
        bool requiresQuoting = DImplementation->QuoteAll || 
                               fieldValue.find(DImplementation->Delimiter) != std::string::npos || 
                               fieldValue.find('\"') != std::string::npos || 
                               fieldValue.find('\n') != std::string::npos;

        if (requiresQuoting) {
            std::ostringstream quotedField;
            quotedField << '\"';
            for (char character : fieldValue) {
                if (character == '\"') {
                    quotedField << "\"\""; 
                } else {
                    quotedField << character;
                }
            }
            quotedField << '\"';
            formattedFields.push_back(quotedField.str());
        } else {
            formattedFields.push_back(fieldValue);
        }
    }

    std::string formattedRow;
    for (size_t i = 0; i < formattedFields.size(); ++i) {
        if (i > 0) {
            formattedRow += DImplementation->Delimiter; // Add delimiter between fields
        }
        formattedRow += formattedFields[i];
    }

    formattedRow += '\n'; // Append newline at the end

    return DImplementation->Sink->Write(std::vector<char>(formattedRow.begin(), formattedRow.end())); // Write to sink
}
