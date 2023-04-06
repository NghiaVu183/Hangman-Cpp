#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

string normalize(const string& s)
{
    string news = s;
    transform(s.begin(), s.end(), news.begin(), ::toupper);
    return news;
}

string chooseWord(const string& fileName, int difficult)
{
    vector<string> vocabulary;
    ifstream file("words\\" + fileName);
    if (!file.is_open())
        return "";

    while (!file.eof())
    {
        string line;
        getline(file, line);
        if (file && !line.empty())
            vocabulary.push_back(normalize(line));
    }

    int n = vocabulary.size();
    string word = vocabulary[rand() % n];
    return n > 0 ? ((word.length() > 5 && difficult) || (word.length() < 5 && !difficult) ? word : chooseWord(fileName, difficult)) : "";
}

bool contains(string word, char guess)
{
    return (word.find(guess) != string::npos);
}
