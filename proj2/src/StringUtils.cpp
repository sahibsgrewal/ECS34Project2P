#include "StringUtils.h"
#include <iostream>
#include <algorithm>
namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{

    ssize_t len = str.length();
    if(str.empty()) return str;
    if (start < 0) start += len;
    if (end == 0) end = len;
    if (end < 0) end += len;

    return str.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    if (str.empty()) return str;
    std::string result = str;
    result[0] = std::toupper(result[0]);
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

std::string Upper(const std::string &str) noexcept{
    std::string result = str;

    for(char &ch: result){
        ch = std::toupper(static_cast<unsigned char>(ch));
    }
    
    return result;
    
}

std::string Lower(const std::string &str) noexcept {
    std::string result = str; 

    for (char &ch : result) { 
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return result;
}

std::string LStrip(const std::string &str) noexcept{
    

    size_t start = str.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start);
    //str.substr(start) removes any whitespace from beginning of the string 
}

std::string RStrip(const std::string &str) noexcept{

    size_t end = str.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);

}

std::string Strip(const std::string &str) noexcept{
    return LStrip(RStrip(str));
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.size();
    if (padding <= 0) return str;
    int pad_left = padding / 2;
    int pad_right = padding - pad_left;
    return std::string(pad_left, fill) + str + std::string(pad_right, fill);
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.size();
    if (padding <= 0) return str;
    return str + std::string(padding, fill);
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.size();
    if (padding <= 0) return str;
    return std::string(padding, fill) + str;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(old, pos)) != std::string::npos) {
        result.replace(pos, old.length(), rep);
        pos += rep.length();
    }
    //If old is found we replace it with rep using replace().
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
    
    std::string result = vect[0];
    for (size_t i = 1; i < vect.size(); ++i) {
        result += str + vect[i];
    }
    
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
    int m = left.length();
    int n = right.length();
    std::string l = left;
    std::string r = right;

    if(ignorecase){
        l = StringUtils::Lower(left);
        r = StringUtils::Lower(right);
    }
    std::vector<std::vector<int>> d(m+1,std::vector<int>(n+1));

    for(int i = 0; i<= m; i++){
        d[i][0] = i;
    }
    for(int i =0; i<=n; i++){
        d[0][i] = i;
    }
    int sc;
    for(int j = 1 ; j<=n; j++){
        for(int i = 1; i <= m; i++){
                if(l[i-1] == r[j-1]){//wikipedia pseudocode started at 1 and not 0
                    sc = 0;

                } 
                else{
                    sc = 1;
                
                }
                d[i][j] = std::min(d[i-1][j]+1,std::min(d[i][j-1]+1,d[i-1][j-1]+sc));
        }
    }


    return d[m][n];
}

//Here is the wiki pseudocode I used for the edit distance function
/*
function LevenshteinDistance(char s[1..m], char t[1..n]):
  // for all i and j, d[i,j] will hold the Levenshtein distance between
  // the first i characters of s and the first j characters of t
  declare int d[0..m, 0..n]
 
  set each element in d to zero
 
  // source prefixes can be transformed into empty string by
  // dropping all characters
  for i from 1 to m:
    d[i, 0] := i
 
  // target prefixes can be reached from empty source prefix
  // by inserting every character
  for j from 1 to n:
    d[0, j] := j
 
  for j from 1 to n:
    for i from 1 to m:
      if s[i] = t[j]:
        substitutionCost := 0
      else:
        substitutionCost := 1

      d[i, j] := minimum(d[i-1, j] + 1,                   // deletion
                         d[i, j-1] + 1,                   // insertion
                         d[i-1, j-1] + substitutionCost)  // substitution
 
  return d[m, n]

*/
};