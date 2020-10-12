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

vector<string> cris_utils::strSplit(string str, string token) {
    vector<string> result;
    while (!str.empty()) {
        int index = str.find(token);
        if (index != string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.empty())
                result.push_back(str);
        } else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
