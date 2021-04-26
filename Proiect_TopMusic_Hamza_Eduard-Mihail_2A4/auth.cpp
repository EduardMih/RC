#include "auth.h"




int Users::checkString(const string str)
{
    regex reg ("([a-zA-Z0-9_]{3,15})");

    if(regex_match(str, reg) != 0)
        
        return 1;

    return 0;
    
}

int Users::putData(const string& username, const string& password, int privileges)
{
    if((checkString(username) == 1) && (checkString(password) == 1))
    {
        this->userName = username;
        this->password = password;
        this->privileges = privileges;

        return 1;

    } 

    return 0;

}

string Users::getUsername()
{
    
    return this->userName;

}

string Users::getPassword()
{

    return this->password;
    
}

int Users::getPriv()
{

    return this->privileges;

}

void Users::putExtraData(int id, int priv)
{
    this->id = id;
    this->privileges = priv;
}

int Users::getUserId()
{

    return this->id;

}