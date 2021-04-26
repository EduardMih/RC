#include <QMainWindow>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <regex>

using namespace std;




class MyClient
{
public:
    int conect();
    void mySend(int sd, const char buf[256]);
    string myRecv(int sd);
    int login(int sd, const char username[256], const char password[256]);
    string createUser(int sd, const char username[256], const char password[256], int type);
    int checkString(const string& str);
    vector<string> showTop(int sd);
    string vote(int sd, const char nume[256]);
    string addSong(int sd, const char name[256], const char description[256], const char link[256], const char genres[256]);
    vector<string> getSongInfo(int sd, const char name[256]);
    vector<string> getUsers(int sd, int size);
    int getAdminInfo(int sd);
    string restrictVote(int sd, const char usernmae[256]);
    string giveVotePerm(int sd, const char usernmae[256]);
    string deleteSong(int sd, const char songName[256]);
    vector<string> showTopOnGenre(int sd, const char genre[256]);
    vector<string> showComments(int sd, const char songName[256]);
    string postComment(int sd, const char songName[256], const char comment[256]);
    void logout(int sd);

};
