#pragma once

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

namespace cris_utils {
    template<typename T>
    bool vectorContains(vector<T> v, T element);

    template<typename T>
    bool compare(std::vector<T> &v1, std::vector<T> &v2);

    vector<string> strSplit(string str, const string &delim);
    void trim(std::string &s);
    bool isNumber(const string &str);

    template<typename T>
    void removeElement(vector<T> &v, T element);

}
