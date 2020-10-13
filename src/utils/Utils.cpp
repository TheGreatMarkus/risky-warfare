#include <algorithm>

#include "Utils.h"

using std::find;
using std::vector;
using std::sort;
using std::string;


template<typename T>
bool cris_utils::vectorContains(vector<T> v, T element) {
    return find(v.begin(), v.end(), element) != v.end();
}

template bool cris_utils::vectorContains(vector<int> v, int element);
template bool cris_utils::vectorContains(vector<bool> v, bool element);

template<typename T>
bool cris_utils::compare(vector<T> &v1, vector<T> &v2) {
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    return v1 == v2;
}

template bool cris_utils::compare(vector<int> &v1, vector<int> &v2);


// Implementation for strSplit taken from
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
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

// Implementation for trim taken from
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
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

void cris_utils::trim(string &s) {
    ltrim(s);
    rtrim(s);
}

// Implementation for isNumber taken from
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool cris_utils::isNumber(const string &str) {
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

template<typename T>
void cris_utils::removeElement(vector<T> &v, T element) {
    v.erase(remove(v.begin(), v.end(), element), v.end());
}

template void cris_utils::removeElement(vector<int> &v, int element);
template void cris_utils::removeElement(vector<void *> &v, void * element);