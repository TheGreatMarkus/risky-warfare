#pragma once

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

namespace cris_utils {
    const int NO_PLAYER_ID = -1;

    template<class T>
    bool vectorContains(vector<T> v, T element);

    template<typename T>
    bool compare(std::vector<T> &v1, std::vector<T> &v2);

    vector<string> strSplit(string str, const string &delim);
    void trim(std::string &s);
    bool isNumber(const string &str);

}
