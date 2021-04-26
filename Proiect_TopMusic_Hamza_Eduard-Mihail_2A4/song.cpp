#include "song.h"




string Song::getDescription()
{

    return this->description;

}

string Song::getLink()
{

    return this->link;

}

string Song::getName()
{
    
    return this->name;

}

void Song::putData(const string& name, const string& description, const string& link, const string& genres)
{
    string intermediate;
    stringstream check (genres);

    this->name = name;
    this->description = description;
    this->link = link;
    
    while(getline(check, intermediate, ','))
        this->genres.push_back(intermediate);


}

vector<string> Song::getGenres()
{

    return this->genres;

}
