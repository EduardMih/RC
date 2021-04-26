#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <regex>

using namespace std;




class Users
{
    int id;
    int privileges;
    string userName = "";
    string password = "";
    
    public:
    int putData(const string& username, const string& password, int privileges=0);
    string getUsername();
    string getPassword();
    int getPriv();
    void putExtraData(int id, int priv);
    int checkString(const string str);
    int getUserId();


};
