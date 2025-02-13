#include "StringUtils.h"
#include <algorithm>
#include <iostream>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    if (end == 0) end = str.size();
    if (start < 0) start += str.size();
    if (end < 0) end += str.size();
    if (start > end) return "";
    return str.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    if (str.empty()) return ""; 
    std::string result = str;
    result[0] = std::toupper(result[0]); // Capitalize  first letter
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = std::tolower(result[i]); // Convert rest to lower
    }
    return result;
}

std::string Upper(const std::string &str) noexcept{
    std::string result = str; // Creates copy
    for (char& c : result) c = toupper(c); 
    return result;
}

std::string Lower(const std::string &str) noexcept{
    std::string result = str;
    for (char& c : result) c = tolower(c);
    return result;
}

std::string LStrip(const std::string &str) noexcept{
    auto pos = str.find_first_not_of(" \t\n\r"); // Finds last char w/o whitespace
    return (pos == std::string::npos) ? "" : str.substr(pos);; // Returns string from first non whitespace
}

std::string RStrip(const std::string &str) noexcept{
    auto pos = str.find_last_not_of(" \t\n\r");
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}

std::string Strip(const std::string &str) noexcept{
    return LStrip(RStrip(str)); // Combine both func
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    if (width <= str.length()) return str; // If width asked for is smaller than string, return org
    int left = (width - str.length()) / 2; // Calculate left
    int right = width - str.length() - left; // Calculate right
    return std::string(left, fill) + str + std::string(right, fill); // Return w fill 
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    if (width <= str.length()) return str;
    return str + std::string(width - str.length(), fill);
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    if (width <= str.length()) return str;
    return std::string(width - str.length(), fill) + str;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(old, pos)) != std::string::npos) { // Find and replace old w rep
        result.replace(pos, old.length(), rep);
        pos += rep.length(); // Move pos past replacement
    }
    return result;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std::vector<std::string> result;
    if (str.empty()) return result;
    if (splt.empty()) { // If string is empty split @ whitespace
        std::string word;
        for (char c : str) {
            if (std::isspace(c)) {
                if (!word.empty()) { // If theres a word, add to results and clear
                    result.push_back(word);
                    word.clear();
                }
            } else {
                word += c; // Add non whitespace to word
            }
        }
        if (!word.empty()) result.push_back(word); // Add last word
        return result;
    }
    size_t pos = 0; // Split @ delimiter
    while (pos < str.length()) {
        size_t next = str.find(splt, pos); // Find next delimeter
        if (next == std::string::npos) {
            result.push_back(str.substr(pos)); // Add remaining string
            break;
        }
        result.push_back(str.substr(pos, next - pos)); // Add substring between pos and delimiter
        pos = next + splt.length();
    }
    return result;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    if (vect.empty()) return "";
    std::string result = vect[0]; // Start @ first string
    for (size_t i = 1; i < vect.size(); i++) result += str + vect[i]; // Add delimeter amd following strings
    return result;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string result;
    int col = 0; // Tracks  current position of column 

    for (char c : str) {
        if (c == '\t') {
            if (tabsize > 0) {
                int spaces = tabsize - (col % tabsize); // Calculate space needed
                result.append(spaces, ' '); // Append required space
                col += spaces; // Update pos
            }
        } else {
            result += c;
            col = (c == '\n') ? 0 : col + 1; // Reset column if newline 
        }
    }
    return result;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
   std::string s1 = left;
    std::string s2 = right;
    
    if (ignorecase) { // Convert to lower if ignorecase is true
        for (char &c : s1) c = tolower(c);
        for (char &c : s2) c = tolower(c);
    }
    
    std::vector<std::vector<int>> d(s1.length() + 1, std::vector<int>(s2.length() + 1)); // Create matrix
    
    for (int i = 0; i <= s1.length(); i++) d[i][0] = i; // Initialize first row and column
    for (int j = 0; j <= s2.length(); j++) d[0][j] = j;
    
    for (int i = 1; i <= s1.length(); i++) { // Fill in matrix
        for (int j = 1; j <= s2.length(); j++) {
            if (s1[i-1] == s2[j-1]) {
                d[i][j] = d[i-1][j-1]; // If char match, copy diagonal
            } else { // if char doesn't match, take min of surrounding and add 1
                    // for insert delete or substitute
                d[i][j] = 1 + std::min(std::min(d[i-1][j], d[i][j-1]), d[i-1][j-1]);
            }
        }
    }
    
    return d[s1.length()][s2.length()]; // return bottom right value
}
};