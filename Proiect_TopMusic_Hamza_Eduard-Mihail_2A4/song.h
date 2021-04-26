#include <iostream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;




class Song
{
    int votes;
    int songId;
    string name;
    string description;
    string link;
    vector<string> genres;

public:
    map<string, int> allGenres = {{"pop", 1}, {"dance", 2}, {"house", 3}, {"electronic", 4}, {"rock", 5}, {"jazz", 6}, {"blues", 7}, {"techno", 8}, {"rap", 9}, {"hip-hop", 10}};
    string getName();
    string getDescription();
    string getLink();
    void putData(const string& name, const string& description, const string& link, const string& genres);
    vector<string> getGenres();

};
