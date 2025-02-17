#include "DSVReader.h"

struct CDSVReader::SImplementation {
    char delimiter;
    std::shared_ptr<CDataSource> dc;

    SImplementation(std::shared_ptr<CDataSource> dc, char delimiter) 
        : delimiter(delimiter == '"' ? ',' : delimiter), dc(dc) {}

    // Checks if reached end
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
                // Handle quotes
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
                    val += a; // Escaped quote
                    continue;
                }
            }
            else if (a == delimiter && !inQuotes) {
                // Delimiter found outside quote, end field
                row.push_back(val);
                val.clear();
            }
            else if (a == '\n') {
                // End of row
                break;
            }
            else {
                // Reg char added to field
                val += a;
            }
        }

        // Push last value if not empty
        if (!val.empty() || !row.empty()) {
            row.push_back(val);
        }

        return !row.empty();
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> dc, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(dc, delimiter)) {}

CDSVReader::~CDSVReader() = default;

// Checks if end of file has been reached
bool CDSVReader::End() const {
    return DImplementation->End();
}

// Reads row from the CSV and stores it in vector
bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    return DImplementation->ReadRow(row);
}