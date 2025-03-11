#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include "ladder.h"
#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

using namespace std;

void error(string word1, string word2, string msg) {
    cerr << msg << endl;
    cerr << "Caused by: " << word1 << ", " << word2 << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (str1.length() == str2.length()) {
        int diff = 0;
        for (int i = 0; i < str1.length(); i++) {
            if (str1[i] != str2[i]) { diff++; }
        }
        return diff == d;
    }
    else if (str1.length() < str2.length()) { return edit_distance_within(str2, str1, d); }
    else {
        // case where str1 > str2
        int skips = 0;
        int i = 0;
        while (i < str2.length()) {
            if ((i+skips) >= str1.length()) { return false; }
            if (str1[i + skips] == str2[i]) { i++; }
            else { skips++; }
        }
        if (skips == 0) { return str1.length() == (str2.length() + d); }
        return (skips == d) && (skips + str2.length() == str1.length());
    }
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) { return {}; }
    if (word_list.find(end_word) == word_list.end()) { return {}; }
    queue<vector<string>> q;
    set<string> visited;  
    q.push({begin_word});
    visited.insert(begin_word);
    while (!q.empty()) {
        int q_size = q.size();
        set<string> q_visited;
        for (int i = 0; i < q_size; i++) {
            vector<string> path = q.front();
            q.pop();
            string last = path.back();
            if (last == end_word) { return path; }
            for (auto word: word_list) {
                if (visited.find(word) == visited.end() && is_adjacent(last, word)) {
                    vector<string> new_path = path;
                    new_path.push_back(word);
                    q.push(new_path);
                    q_visited.insert(word);
                }
            }
        }
        for (auto w : q_visited) {
            visited.insert(w);
        }
    }
    return {};
}

void load_words(set<string> & word_list, const string& file_name) {
    ifstream in(file_name);
    string word;
    while (in >> word) {
        word_list.insert(word);
    }
    in.close();
}

void print_word_ladder(const vector<string>& ladder) {
    for (auto word: ladder) {
        cout << word << ' ';
    }
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}