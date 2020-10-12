#include <algorithm>

#include "Utils.h"

using std::find;


template<class T>
bool cris_utils::vectorContains(vector<T> v, T element) {
    return find(v.begin(), v.end(), element) != v.end();
}

template bool cris_utils::vectorContains(vector<int> v, int element);
template bool cris_utils::vectorContains(vector<bool> v, bool element);

template<typename T>
bool cris_utils::compare(std::vector<T> &v1, std::vector<T> &v2) {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}

template bool cris_utils::compare(std::vector<int> &v1, std::vector<int> &v2);


// Implementation for strSplit taken from
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<string> cris_utils::strSplit(string str, const string &delimiter) {
    vector<string> result;
    size_t pos = 0;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);
    return result;
}

// Implementation for trim taken from
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void cris_utils::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}