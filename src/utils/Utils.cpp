#include "Utils.h"

#include <algorithm>


using std::find;
using std::vector;
using std::sort;
using std::string;

namespace {
    void ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
    }

    void rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), s.end());
    }
}


/**
 * Splits a string given a delimiter
 *
 * Implementation taken from
 * https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 * @param str
 * @param delimiter
 * @return vector of tokens
 */
vector<string> cris_utils::strSplit(string str, const string &delimiter) {
    vector<string> result;
    size_t pos = 0;
    while ((pos = str.find(delimiter)) != string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);
    return result;
}

/**
 * Trims string of whitespace
 *
 * Implementation taken from
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 * @param s string to trim
 */
void cris_utils::trim(string &s) {
    ltrim(s);
    rtrim(s);
}

/**
 * Checks if string can be converted to a number
 *
 * Implementation for isNumber taken from
 * https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
 * @param str
 * @return whether string can be converted to a number
 */

bool cris_utils::isNumber(const string &str) {
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}