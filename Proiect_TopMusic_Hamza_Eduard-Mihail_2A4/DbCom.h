#include <iostream>
#include <sqlite3.h>
#include "auth.h"
#include "song.h"


using namespace std;




class DbComm
{
    sqlite3 *DB;
    int checkUser(Users& user);

    public:
    int open();
    int close();
    int addSong(Song& newSong);
    int authentificate(Users& user);
    int createUser(Users& user);
    void getUserInfo(Users& user);
    int voteSong(const string& songName);
    vector<string> showTop();
    string getSongDescr(const string& songName);
    string getSongGenres(const string& songName);
    string getSongLink(const string& songName);
    int restricUser(const string& userName);
    int giveVotePerm(const string& userName);
    vector<string> getNormalUsers();
    int deleteSong(const string& songName);
    vector<string> showTopOnGenres(const string& genre);
    int postComment(int user_id, const string& comment, const string& songName);
    vector<string> getComments(const string& songName);
    

};
