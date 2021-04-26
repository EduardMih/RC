#include "MyClient.h"




int MyClient::conect()
{
    int sd, port;
    struct sockaddr_in server;

    port = 2024;

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
         printf("%s", "Erare la socket\n");

     server.sin_family = AF_INET;
     server.sin_addr.s_addr = inet_addr("127.0.0.1");
     server.sin_port = htons(port);

     if(connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1)
     {
         printf("%s", "Eroare la conect\n");

         return -1;
     }

     return sd;

}

void MyClient::mySend(int sd, const char buf[256])
{
    int length;

    length = strlen(buf);

    if(-1 == send(sd, &length, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

     if(-1 == send(sd, buf, length, 0))
        printf("%s", "Eroare la send\n");
}

string MyClient::myRecv(int sd)
{
    int length;
    char buf[256];
    string msg;

    if(recv(sd, &length, sizeof(int), 0) == -1)
         printf("[Server - %d] %s", getpid(), "Eroare la recv");

    if(recv(sd, buf, length, 0) == -1)
         printf("[Server - %d] %s", getpid(), "Eroare la recv");

    buf[length] = '\0';
    msg = buf;

        return msg;

}

int MyClient::login(int sd, const char username[256], const char password[256])
{
    int logged, option = 1;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, username);
    mySend(sd, password);

    if(recv(sd, &logged, sizeof(int), 0) == -1)
        printf("%s", "Eroare la recv\n");

    return logged;

}

string MyClient::createUser(int sd, const char username[256], const char password[256], int type)
{
    string str;

    if(-1 == send(sd, &type, sizeof(int), 0))
        printf("%s", "Eroare la send\n");


    mySend(sd, username);
    mySend(sd, password);

    str = myRecv(sd);

    return str;

}

int MyClient::checkString(const string &str)
{
    regex reg ("([a-zA-Z0-9_]{3,15})");

    if(regex_match(str, reg) != 0)

        return 1;

    return 0;

}

vector<string> MyClient::showTop(int sd)
{
    int size, option=6;
    string topLine;
    vector<string> top;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

     for(int i = 0; i < size; i++)
     {
         topLine = myRecv(sd);
         top.push_back(topLine);
     }

     return top;

}

vector<string> MyClient::showTopOnGenre(int sd, const char genre[256])
{
    int size, option=7;
    string topLine;
    vector<string> top;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, genre);

    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

     for(int i = 0; i < size; i++)
     {
         topLine = myRecv(sd);
         top.push_back(topLine);
     }

     return top;

}

string MyClient::vote(int sd, const char nume[256])
{
    int option=5;
    string str;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, nume);

    str = myRecv(sd);

    return str;

}

string MyClient::addSong(int sd, const char name[256], const char description[256], const char link[256], const char genres[256])
{
    int option=4;
    string str;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, name);
    mySend(sd, description);
    mySend(sd, link);
    mySend(sd, genres);

    str = myRecv(sd);

    return str;
}

vector<string> MyClient::getSongInfo(int sd, const char name[256])
{
    int option=11;
    vector<string> info;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, name);

    info.push_back(myRecv(sd));
    info.push_back(myRecv(sd));
    info.push_back(myRecv(sd));

    return info;
}

int MyClient::getAdminInfo(int sd)
{
    int option=12, size;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    if(recv(sd, &size, sizeof(int), 0) == -1)
            printf("[Client - %d] %s", getpid(), "Eroare la recv");

    return size;

}

vector<string> MyClient::getUsers(int sd, int size)
{
    vector<string> users;

    for(int i = 0; i < size; i++)
        users.push_back(myRecv(sd));

    return users;

}

string MyClient::restrictVote(int sd, const char username[256])
{
    int opt=1;
    string resp;

    mySend(sd, username);

    if(-1 == send(sd, &opt, sizeof(int), 0))
         printf("%s", "Eroare la send\n");

     resp = myRecv(sd);

     return resp;

}

string MyClient::giveVotePerm(int sd, const char username[256])
{
    int opt=2;
    string resp;

    mySend(sd, username);

    if(-1 == send(sd, &opt, sizeof(int), 0))
         printf("%s", "Eroare la send\n");

     resp = myRecv(sd);

     return resp;

}

string MyClient::deleteSong(int sd, const char songName[256])
{
    int option=9;
    string res;


    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, songName);
    res = myRecv(sd);

    return res;

}

vector<string> MyClient::showComments(int sd, const char songName[256])
{
    int option=13, size;
    vector<string> comments;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, songName);

    if(recv(sd, &size, sizeof(int), 0) == -1)
       printf("[Client - %d] %s", getpid(), "Eroare la recv");

    for(int i = 0; i < size; i++)
       comments.push_back(myRecv(sd));

    return comments;

}

string MyClient::postComment(int sd, const char songName[256], const char comment[256])
{
    int option=8;
    string resp;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

    mySend(sd, songName);
    mySend(sd, comment);

    resp = myRecv(sd);

    return resp;

}

void MyClient::logout(int sd)
{
    int option=14;

    if(-1 == send(sd, &option, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

}

