#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <map>

using std::string;
using std::vector;
using std::ostream;
using std::set;
using std::map;
using std::cout;
using std::find;
using std::endl;
using std::is_pointer;

/**
 * Utilities used throughout the program
 *
 * Template functions are included inline, due to the their limitations
 * in C++: https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
 *
 */
namespace cris_utils {
    /**
     * Checks if a vector contains an element.
     *
     * @return whether vector contains the element
     */
    template<typename T>
    bool contains(vector<T> v, T element) {
        return find(v.begin(), v.end(), element) != v.end();
    }

    template<typename T>
    bool contains(set<T> s, T element) {
        return s.find(element) != s.end();
    }

    template<typename T, typename S>
    bool contains(map<T, S> s, T element) {
        return s.find(element) != s.end();
    }

    template<typename T>
    set<T> vectorToSet(vector<T> v) {
        return set<T>(v.begin(), v.end());
    }

    template<typename T>
    vector<T> setToVector(set<T> s) {
        return vector<T>(s.begin(), s.end());
    }

    /**
     * Compares two vectors and returns whether they have the same elements.
     * @return Whether they have the same elements
     */
    template<typename T>
    bool compare(std::vector<T> &v1, std::vector<T> &v2) {
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        return v1 == v2;
    }

    template<typename T>
    bool compare(std::set<T> &s1, std::set<T> &s2) {
        return s1 == s2;
    }

    vector<string> strSplit(string str, const string &delim);
    void trim(std::string &s);
    bool isNumber(const string &str);

    /**
     * Remove element from vector
     */
    template<typename T>
    void removeElement(vector<T> &v, T element) {
        v.erase(remove(v.begin(), v.end(), element), v.end());
    }

    /**
     * Remove element from set
     */
    template<typename T>
    void removeElement(set<T> &s, T element) {
        s.erase(element);
    }

    int getIntInput(string prompt, int min, int max);
    int getBoolInput(string prompt);

    void printTitle(string title);
    void printSubtitle(string title);

    int randInt(int min, int max);

    template<typename T>
    void printList(vector<T> list) {
        for (int i = 0; i < list.size(); ++i) {
            cout << "\t" << (i + 1) << ": " << list[i] << endl;
        }
    }

    template<typename T>
    void printList(vector<T *> list) {
        for (int i = 0; i < list.size(); ++i) {
            cout << "\t" << (i + 1) << ": " << *(list[i]) << endl;
        }
    }

    template<typename T>
    T pickFromList(string desc, string prompt, vector<T> list) {
        cout << desc << endl;
        printList(list);
        return list[getIntInput(prompt, 1, list.size()) - 1];
    }
}

