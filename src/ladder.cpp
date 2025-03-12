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
        return diff <= d;
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
        if (skips == 0) { return str1.length() >= (str2.length() + d); }
        return (skips <= d) && (skips + str2.length() <= str1.length());
    }
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) { return {}; }
    if (word_list.find(end_word) == word_list.end()) { return {}; }

    queue<vector<string>> q;
    q.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);

    while (!q.empty()) {
        vector<string> ladder = q.front();
        q.pop();
        string last = ladder.back();

        for (auto word: word_list) {
            if (is_adjacent(last, word) && visited.find(word) == visited.end()) {
                vector<string> new_path = ladder;
                new_path.push_back(word);
                visited.insert(word);
                if (word == end_word) { return new_path; }
                q.push(new_path);
            }
        }
    }
    return {};
}

void load_words(set<string> & word_list, const string& file_name) {
    ifstream in(file_name);
    string word;
    if (!in) {
        cerr << "Error opening file: " << file_name << endl;
        return;
    }
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
    load_words(word_list, "src/words.txt");
    for (auto i: word_list) 
        cout << i << ' ';
    // my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    // cout << generate_word_ladder("cat", "dog", word_list).size() << endl;
    // my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    // cout << generate_word_ladder("marty", "curls", word_list).size() << endl;
    // my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    // cout << generate_word_ladder("code", "data", word_list).size() << endl;
    // my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    // cout << generate_word_ladder("work", "play", word_list).size() << endl;
    // my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    // cout << generate_word_ladder("sleep", "awake", word_list).size() << endl;
    // my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
    // cout << generate_word_ladder("car", "cheat", word_list).size() << endl;
}