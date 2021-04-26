#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DbCom.h"





int logged = 0;
Users user;
char notLogged[26] = "ATENTIE nu sunteti logat";

string myRecv(int client)
{
    int length;
    char buf[256];
    string msg;

    if(recv(client, &length, sizeof(int), 0) == -1)
        printf("[Server - %d] %s", getpid(), "Eroare la recv");

    if(recv(client, buf, length, 0) == -1)
        printf("[Server - %d] %s", getpid(), "Eroare la recv");

    buf[length] = '\0';
    msg = buf;

    return msg;
}


void mySend(int client, const char buf[256])
{
    int length;

    length = strlen(buf);

     if(-1 == send(client, &length, sizeof(int), 0))
        printf("%s", "Eroare la send\n");
    
     if(-1 == send(client, buf, length, 0))
        printf("%s", "Eroare la send\n");
}


void createAdmin(int client)
{
    Users auxUser;
    DbComm myDataBase;
    string username, password;

    username = myRecv(client);
    password = myRecv(client);

    auxUser.putData(username, password, 2);

    if(myDataBase.createUser(auxUser) == 1)
        mySend(client, "Admin creat cu succes");
    
    else

        mySend(client, "Adminul nu a putut fi creat");

}


void createUser(int client)
{
    Users auxUser;
    DbComm myDataBase;
    string username, password;

    username = myRecv(client);
    password = myRecv(client);

    auxUser.putData(username, password, 1);

    
    if(myDataBase.createUser(auxUser) == 1)
        mySend(client, "User creat cu succes");

    else

        mySend(client, "Userul nu a putut fi creat");

}


void login(int client)
{
    DbComm myDataBase;
    string username, password;

    
    username = myRecv(client);
    password = myRecv(client);

    user.putData(username, password);

    if(myDataBase.authentificate(user) == 1)
        logged = 1;
    

    if(send(client, &logged, sizeof(int), 0) == -1)
        printf("[Server - %d] %s", getpid(), "Eroare la send");

    
}

void logout(int client)
{
    logged=0;
    user.putData("", "", 0);
    user.putExtraData(-1, 0);
}


void addSong(int client)
{
    Song newSong;
    DbComm myDataBase;
    string name, description, link, genres;



    name = myRecv(client);
    description = myRecv(client);
    link = myRecv(client);
    genres = myRecv(client);

    newSong.putData(name, description, link, genres);
    
    if(myDataBase.addSong(newSong) == 0)
        mySend(client, "Melodie adaugata cu succes");

    else

       mySend(client, "Esec la adaugare melodie");
    

   

    cout<<user.getUsername()<<" "<<user.getPriv()<<endl;

}

void voteSong(int client)
{
   DbComm myDataBase;
   string songName;

   songName = myRecv(client);

   myDataBase.getUserInfo(user);

   cout<<"Priv:"<<user.getPriv()<<endl;

   if((user.getPriv() != 0) && (myDataBase.voteSong(songName) == 0))
        mySend(client, "Melodie votata cu succes");

    else

        mySend(client, "Esec la votare");
    

}


void showTop(int client)
{
   DbComm myDataBase;
   int size;
   vector<string> top;

   top = myDataBase.showTop();
   size = top.size();

   if(-1 == send(client, &size, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

   for(int i = 0; i < size; i++)
        mySend(client, top[i].c_str());



}

void showGenTop(int client)
{
   DbComm myDataBase;
   int size;
   string genre;
   vector<string> top;

   genre = myRecv(client);

   top = myDataBase.showTopOnGenres(genre);
   size = top.size();

   if(-1 == send(client, &size, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

   for(int i = 0; i < size; i++)
        mySend(client, top[i].c_str());

}

void postComm(int client)
{
    DbComm myDataBase;
    string songName, comment;

    songName = myRecv(client);
    comment = myRecv(client);

    if(myDataBase.postComment(user.getUserId(), comment, songName) == 0)
        mySend(client, "Comentariu adaugat cu succes");

    else

       mySend(client, "Eroare la adaugare comentariu");

}

void showComments(int client)
{
   DbComm myDataBase;
   int size;
   string songName;
   vector<string> comments;

   songName = myRecv(client);

   comments = myDataBase.getComments(songName);
   size = comments.size();

   if(-1 == send(client, &size, sizeof(int), 0))
        printf("%s", "Eroare la send\n");

   for(int i = 0; i < size; i++)
        mySend(client, comments[i].c_str());
}

void deleteSong(int client)
{
    DbComm myDataBase;
    string name;

    name = myRecv(client);

    if(user.getPriv() == 2)
    {
        if(myDataBase.deleteSong(name) == 0)
            mySend(client, "Melodie stearsa cu succes");

        else
  
           mySend(client, "Eroare la stergere melodie");
    
    }

    else

        mySend(client, "Nu sunteti admin");
        
}



void changeVotePerm(int client)
{
    DbComm myDataBase;
    int size, opt;
    string name;
    vector<string> users;

    if(user.getPriv() == 2)
    {
        users = myDataBase.getNormalUsers();
        size = users.size();

        if(-1 == send(client, &size, sizeof(int), 0))
            printf("%s", "Eroare la send\n");

        for(int i = 0; i < size; i++)
            mySend(client, users[i].c_str());
    
        name = myRecv(client);


        if(recv(client, &opt, sizeof(int), 0) == -1)
            printf("[Server - %d] %s", getpid(), "Eroare la recv");

        if(opt == 1)
        {
             if(myDataBase.restricUser(name) == 0)
            mySend(client, "Acess restrinctionat cu succes!");

        else

            mySend(client, "Eroare la restr acces");

        }

        if(opt == 2)
        {
            if(myDataBase.giveVotePerm(name) == 0)
                mySend(client, "Acess redat cu succes!");

            else

                mySend(client, "Eroare la redare acces");

        }
    }

    else

        {
            size = -1;

            if(-1 == send(client, &size, sizeof(int), 0))
                printf("%s", "Eroare la send\n");

        }

}

void exitCli(int client)
{
   string msg="Exited";

   mySend(client, msg.c_str());

}


int readOption(int client)
{
    int option;

    if(recv(client, &option, sizeof(int), 0) == -1)
        printf("[Server - %d] %s", getpid(), "Erare la recv");

    return option;
    
}

void sendInfo(int client)
{
    DbComm myDataBase;
    string name, description, genres, link;

    name = myRecv(client);

    description = myDataBase.getSongDescr(name);
    genres = myDataBase.getSongGenres(name);
    link = myDataBase.getSongLink(name);

    mySend(client, description.c_str());
    mySend(client, genres.c_str());
    mySend(client, link.c_str());

}

int main()
{
    int sd, client, option=0;
    struct sockaddr_in server;
    struct sockaddr_in from;
    pid_t pid;
    socklen_t length;   


    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("%s", "Erare la socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(2024);

    if(bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
        printf("%s", "Eroare la bind");

    
    if(listen(sd, 5) == -1)
        printf("%s", "Eraore la listen");

    length = sizeof(from);
    
    while(1)
    {
        if((client = accept(sd, (struct sockaddr*) &from, &length)) < 0)
            printf("%s", "Eroare la accept");

        
        if((pid = fork()) < 0)
            printf("%s", "Eroare la fork");
        
        if(pid == 0)
        {
            close(sd);

            
            
            while(1)
            {
                while(logged == 0)
                {
                    option = readOption(client);

                    cout<<"[Server - "<<getpid()<<"] "<<"Primit optiunea "<<option<<endl;

                    if(option == 1)
                        login(client);

                    if (option == 2)
                        createUser(client);

                    if(option == 3)
                        createAdmin(client);
                }

                option = readOption(client);

                cout<<"[Server - "<<getpid() << "] "<< "Primit optiunea " <<option<<endl;


                if (option == 4)
                    addSong(client);

                if (option == 5)
                    voteSong(client);

                if (option == 6)
                    showTop(client);

                if (option == 7)
                    showGenTop(client);

                if (option == 8)
                    postComm(client);

                if (option == 9)
                    deleteSong(client);

                if (option == 10)
                    changeVotePerm(client);

                if(option == 11)
                    sendInfo(client);

                if(option == 12)
                    changeVotePerm(client);

                if(option == 13)
                    showComments(client);

                if(option == 14)
                    logout(client);

                if ((option < 0) || (option > 14))
                    {
                        exitCli(client);

                        return 0;

                    }
            }
        }

        else
        
        {
            close(client);

        }

    }

    return 0;
    
}
