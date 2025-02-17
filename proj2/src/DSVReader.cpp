#include "DSVReader.h"

struct CDSVReader::SImplementation {
    char delimiter;
    std::shared_ptr<CDataSource> dc;

    SImplementation(std::shared_ptr<CDataSource> dc, char delimiter) 
        : dc(dc), delimiter(delimiter == '"' ? ',' : delimiter) {}

// Checks if end is reached
    bool End() const {
        char a;
        return !dc->Peek(a);
    }

    bool ReadRow(std::vector<std::string>& row) {
        row.clear();
        std::string val;
        char a;
        bool inQuotes = false;

        while (dc->Get(a)) {
            if (a == '"') {
                // Handles quotes
                if (val.empty()) {
                    inQuotes = true;
                    continue;
                }
                char nextChar;
                if (!dc->Get(nextChar) || nextChar == '\n') {
                    break;
                }
                if (nextChar == delimiter) {
                    row.push_back(val);
                    val.clear();
                    inQuotes = false;
                    continue;
                }
                if (nextChar == '"') {
                    // Escaped quote
                    val += a;
                    continue;
                }
            }
            else if (a == delimiter && !inQuotes) {
                // Delimeter found outside the quotes, end field
                row.push_back(val);
                val.clear();
            }
            else if (a == '\n') {
                // Its end of row
                break;
            }
            else {
                // Reg char added to field
                val += a;
            }
        }
// Push last val if not empty
        if (!val.empty() || !row.empty()) {
            row.push_back(val);
        }

        return !row.empty();
    }
};
// Initialize implementation
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> dc, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(dc, delimiter)) {}

CDSVReader::~CDSVReader() = default;

// Check if reader reached end of file
bool CDSVReader::End() const {
    return DImplementation->End();
}

// Reads row from CSV and stores in vector
bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    return DImplementation->ReadRow(row);
}
