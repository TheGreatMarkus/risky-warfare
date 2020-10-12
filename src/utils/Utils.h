#pragma once

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;

namespace cris_utils {

    template<class T>
    bool vectorContains(vector<T> v, T element);

    template<typename T>
    bool compare(std::vector<T> &v1, std::vector<T> &v2);

    vector<string> strSplit(string str, string token);

}
