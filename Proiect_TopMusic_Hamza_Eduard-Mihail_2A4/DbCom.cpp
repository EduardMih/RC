#include "DbCom.h"




int DbComm::open()
{
    int status;

    status = sqlite3_open("TopMusicDb.db", &this->DB);

    if(status)
        {
            cout<<"Eroare la deschidere baza de date\n";
            sqlite3_close(this->DB);
        }

    return status;

}

int DbComm::close()
{
    int status;

    status = sqlite3_close(this->DB);

    if(status != SQLITE_OK)
    {
        cout<< "Eroare la inchidere baza de date\n";

        return 1;
    }

    return 0;

}

int DbComm::postComment(int user_id, const string& comment, const string& songName)
{
    int status=SQLITE_BUSY;
    string insert_query = "INSERT INTO comments (song_id, user_id, comment) VALUES ( (SELECT song_id FROM songs WHERE name = ";
    char* ErrorMes;
    
    insert_query = insert_query + "'" + songName + "')" + "," + to_string(user_id) + ", '" + comment + "')"; 

    cout<<insert_query<<endl;
    
    this->open();

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, insert_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la insert comentariu\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    return 0;

}

vector<string> DbComm::getComments(const string& songName)
{
    int status, priv;
    string select_query = "SELECT username, comment FROM users NATURAL JOIN comments WHERE song_id = (SELECT song_id FROM songs WHERE name = ";
    const unsigned char* buf, *buf2;
    string username, comment;
    vector<string> comments;
    sqlite3_stmt* stmt;

    select_query = select_query + "'" + songName + "');";

    this->open();

    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        buf = sqlite3_column_text(stmt, 0);
        buf2 = sqlite3_column_text(stmt, 1);

        username = reinterpret_cast<char const*> (buf);
        comment = reinterpret_cast<char const*>(buf2);
        
        comments.push_back(username + ": " + comment);

    }

        sqlite3_finalize(stmt);

    this->close();

    return comments;
    
}

vector<string> DbComm::getNormalUsers()
{
    int status, priv;
    string select_query = "SELECT username, privileges FROM users WHERE privileges IN (0, 1);";
    string name;
    vector<string> users;
    const unsigned char* buf;
    sqlite3_stmt* stmt;

    this->open();

    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        buf = sqlite3_column_text(stmt, 0);
        priv = sqlite3_column_int(stmt, 1);

        name = reinterpret_cast<char const*> (buf);
        
        if(priv == 0)
            name = name + " (fara drept de vot)";
        
        else

            name = name + " (cu drept de vot)";

        users.push_back(name);

    }

        sqlite3_finalize(stmt);

    this->close();

    return users;

}

int DbComm::restricUser(const string& userName)
{
    int status=SQLITE_BUSY, priv;
    string update_query = "UPDATE users SET privileges = 0 WHERE username = ";
    char* ErrorMes;
   
    
    update_query = update_query + "'" + userName + "';";
    
    this->open();

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, update_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la update drepturi\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    this->close();

    return 0;

}

int DbComm::giveVotePerm(const string& userName)
{
    int status=SQLITE_BUSY, priv;
    string update_query = "UPDATE users SET privileges = 1 WHERE username = ";
    char* ErrorMes;
   
    
    update_query = update_query + "'" + userName + "';";
    
    this->open();

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, update_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la update drepturi\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    this->close();

    return 0;

}

int DbComm::addSong(Song& newSong)
{
    int status=SQLITE_BUSY, last;
    string description, link, name;
    vector<string> genres;
    string insert_query = "INSERT INTO songs (name, description, votes, link) VALUES ( ";
    string select_query = "SELECT song_id FROM songs WHERE name = ";
    char* ErrorMes;
    sqlite3_stmt* stmt;
    
    name = "'" + newSong.getName() + "'";
    description = "'" + newSong.getDescription() + "'";
    link = "'" + newSong.getLink() + "'";
    
    insert_query = insert_query + name + "," + description + "," + "0" + "," + link + ");";

    cout<<insert_query<<endl;
    
    this->open();

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, insert_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la insert\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    select_query = select_query + name;

    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    sqlite3_step(stmt);
    last = sqlite3_column_int(stmt, 0);

    if(last == NULL)
        {
            sqlite3_finalize(stmt);

            return 0;

        }

    sqlite3_finalize(stmt);


    genres = newSong.getGenres();

    for(auto it = genres.begin(); it != genres.end(); it++)
    {
        insert_query = "INSERT INTO song_genre (genre_id, song_id) VALUES (";
        insert_query = insert_query + to_string(newSong.allGenres[*it]) + ",";
        insert_query = insert_query + to_string(last) + ")";

        status = SQLITE_BUSY;
        
        while(status == SQLITE_BUSY)
            status = sqlite3_exec(this->DB, insert_query.c_str(), NULL, 0, &ErrorMes);

        if(status != SQLITE_OK)
        {
            cout<<"Eroare la insert (genuri)\n";
            sqlite3_free(ErrorMes);

            return 1;

        }
    }
    
    this->close();
    
    cout<<"Melodie adaugata cu sucess!\n";

    return 0;
    
}

int DbComm::deleteSong(const string& songName)
{
    int status=SQLITE_BUSY, priv;
    string delete_query = "DELETE FROM songs WHERE name = ";
    string delete_query2 = "DELETE FROM song_genre WHERE song_id = (SELECT song_id FROM songs WHERE name = ";
    string delete_query3 = "DELETE FROM comments WHERE song_id = (SELECT song_id FROM songs WHERE name = ";
    char* ErrorMes;
   
    
    delete_query = delete_query + "'" + songName + "';";
    delete_query2 = delete_query2 + "'" + songName + "');";
    delete_query3 = delete_query3 + "'" + songName + "')";
    this->open();

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, delete_query2.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la sters melodie\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    status = SQLITE_BUSY;
 
    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, delete_query3.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la sters melodie\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    status = SQLITE_BUSY;

    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, delete_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la sters melodie\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }


    this->close();

    return 0;

}

int DbComm::checkUser(Users& user)
{
    int status;
    string select_query = "SELECT pass FROM users WHERE username = ";
    const unsigned char* buf;
    string retPass, username, password;
    sqlite3_stmt* stmt;
    
    username = user.getUsername();
    password = user.getPassword();

    select_query = select_query + "'" + username + "'" + ";";

    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    sqlite3_step(stmt);
    buf = sqlite3_column_text(stmt, 0);

    if(buf == NULL)
        {
            sqlite3_finalize(stmt);

            return 0;

        }

    retPass = reinterpret_cast<char const*> (buf);

    sqlite3_finalize(stmt);

    if(password != retPass)
        
        return 0;

    return 1;

}

int DbComm::authentificate(Users& user)
{
    int k = 0;

    this->open();
    k = this->checkUser(user);
    this->close();
    
    if(k == 1)
        this->getUserInfo(user);
    


    return k;

}

void DbComm::getUserInfo(Users& user)
{
    int id, priv;
    string select_query = "SELECT user_id, privileges FROM users WHERE username = ";
    string retPass, username, password;
    sqlite3_stmt* stmt;
    
    username = user.getUsername();

    select_query = select_query + "'" + username + "'" + ";";
    
    this->open();
    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    sqlite3_step(stmt);
    id = sqlite3_column_int(stmt, 0);
    priv = sqlite3_column_int(stmt, 1);
    
    cout<<"userid "<<id<<endl;
    cout<<"priv from "<<priv<<endl;

    user.putExtraData(id, priv);

    sqlite3_finalize(stmt);

    this->close();
}

int DbComm::createUser(Users& user)
{
    int status = SQLITE_BUSY;
    string insert_query = "INSERT INTO users (username, pass, privileges) VALUES (";
    char* ErrorMes;
 
    this->open();
    
    insert_query = insert_query + "'" + user.getUsername() + "', '" + user.getPassword() + "', " + to_string(user.getPriv()) + ");";
    
    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, insert_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la insert\n";
        sqlite3_free(ErrorMes);

        return 0;

    }

    else
    
        cout<<"User creat cu sucess!\n";

    this->close();

   return 1;
    
}


int DbComm::voteSong(const string& songName)
{
    int status = SQLITE_BUSY;
    string update_query = "UPDATE songs SET votes = votes + 1 WHERE name = ";
    char* ErrorMes;

    update_query = update_query + "'" + songName + "';";

    this->open();
    
    while(status == SQLITE_BUSY)
        status = sqlite3_exec(this->DB, update_query.c_str(), NULL, 0, &ErrorMes);

    if(status != SQLITE_OK)
    {
        cout<<"Eroare la update voturi\n";
        cout<<ErrorMes<<endl;
        sqlite3_free(ErrorMes);

        return 1;

    }

    this->close();

    return 0;

}


vector<string> DbComm::showTop()
{
    int status, votes;
    string select_query = "SELECT name, votes FROM songs ORDER BY votes DESC, name ASC;";
    string topLine;
    vector<string> top;
    const unsigned char* buf;
    sqlite3_stmt* stmt;

    this->open();

    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        buf = sqlite3_column_text(stmt, 0);
        votes = sqlite3_column_int(stmt, 1);

        topLine = reinterpret_cast<char const*> (buf);
        topLine = topLine + " (" + to_string(votes) + " voturi)";

        top.push_back(topLine);

    }

        sqlite3_finalize(stmt);

    this->close();

    return top;

}

vector<string> DbComm::showTopOnGenres(const string& genre)
{
    int status, votes;
    string select_query = "SELECT name, votes FROM songs NATURAL JOIN song_genre WHERE genre_id = (SELECT genre_id FROM genres WHERE name = ";
    string topLine;
    vector<string> top;
    const unsigned char* buf;
    sqlite3_stmt* stmt;

    this->open();

    select_query = select_query + "'" + genre + "');";

    cout<<select_query<<endl;
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        buf = sqlite3_column_text(stmt, 0);
        votes = sqlite3_column_int(stmt, 1);

        topLine = reinterpret_cast<char const*> (buf);
        topLine = topLine + " (" + to_string(votes) + " voturi)";

        top.push_back(topLine);

    }

        sqlite3_finalize(stmt);

    this->close();

    return top;

}

string DbComm::getSongDescr(const string& songName)
{
    int status;
    string select_query = "SELECT description FROM songs WHERE name = ";
    string description;
    const unsigned char* buf;
    sqlite3_stmt* stmt;


    select_query = select_query + "'" + songName + "';";
    
    this->open();
    
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    sqlite3_step(stmt);
    buf = sqlite3_column_text(stmt, 0);

    if(buf == NULL)
        {
            sqlite3_finalize(stmt);

            return 0;

        }

    description = reinterpret_cast<char const*> (buf);

    sqlite3_finalize(stmt);

    this->close();

    return description;

}

string DbComm::getSongGenres(const string& songName)
{
    string genres="";
    string select_query = "SELECT g.name FROM songs s JOIN song_genre sg ON s.song_id = sg.song_id JOIN genres g ON g.genre_id = sg.genre_id WHERE s.name = " ;
    string description;
    const unsigned char* buf;
    sqlite3_stmt* stmt;


    select_query = select_query + "'" + songName + "';";

    this->open();

    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        buf = sqlite3_column_text(stmt, 0);
        
        if(genres == "")
            genres = reinterpret_cast<char const*>(buf);

        else

            genres = genres + "," + reinterpret_cast<char const*>(buf);
    
    }    

    sqlite3_finalize(stmt);

    this->close();

    return genres;

}

string DbComm::getSongLink(const string& songName)
{
    int status;
    string select_query = "SELECT link FROM songs WHERE name = ";
    string link;
    const unsigned char* buf;
    sqlite3_stmt* stmt;


    select_query = select_query + "'" + songName + "';";
    
    this->open();
    
    sqlite3_prepare_v2(this->DB, select_query.c_str(), -1, &stmt, 0);
    
    sqlite3_step(stmt);
    buf = sqlite3_column_text(stmt, 0);

    if(buf == NULL)
        {
            sqlite3_finalize(stmt);

            return 0;

        }

    link = reinterpret_cast<char const*> (buf);

    sqlite3_finalize(stmt);

    this->close();

    return link;
    
}


