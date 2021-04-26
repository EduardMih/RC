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



using namespace std;





int logged=0;
char explic[300] = "Optiuni: 1-Login; 2-create user; 3-create admin; 4-adauga mmelodie; 5-voteaza;\n 6-arata top general; 7-arata top pe gen; 8-adauga comentariu; 9-sterge melodie; 10, 12-restrictioneaza vot; 11-arata info; 13-arata comentarii; 14-logout; 15-exit";

void mySend(int sd, const char buf[256])
{
    int length;

    length = strlen(buf);

     if(-1 == send(sd, &length, sizeof(int), 0))
        printf("%s", "Eroare la send\n");
    
     if(-1 == send(sd, buf, length, 0))
        printf("%s", "Eroare la send\n");
}

string myRecv(int sd)
{
    int length;
    char buf[256];
    string msg;

    if(recv(sd, &length, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

    if(recv(sd, buf, length, 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

    buf[length] = '\0';
    msg = buf;

    return msg;
}

void login(int sd)
{
    char msg[256];

    cout<<"Username: ";
    cin>>msg;

    mySend(sd, msg);

    cout<<"Password: ";
    cin>>msg;

    mySend(sd, msg);

    if(recv(sd, &logged, sizeof(int), 0) == -1)
        cout<<"Eroare la recv\n";

}

void logout(int sd)
{
    logged = 0;
}

void createUser(int sd)
{
    int k = 1;
    char msg[256], msg2[256], msg3[256];
    string str;

    while(k == 1)
    {
        printf("%s", "Username: ");
        scanf("%s", msg);

        printf("%s", "Password: ");
        scanf("%s", msg2);

        printf("%s", "Confirm password: ");
        scanf("%s", msg3);

        if(strcmp(msg2, msg3) == 0)
        {
            mySend(sd, msg);
            mySend(sd, msg2);
            k = 0;
        }

        else

            printf("%s\n", "Parolele nu se potrivesc");
    
    }

    str = myRecv(sd);

    cout<<str<<endl;
    
}

void addSong(int sd)
{
    int k, q = 0;
    char name[256], description[256], link[256], genres[256], copy[256];
    char sep[2] = ",";
    char knownGenres[15] [20] = {"pop", "dance", "house", "electronic", "rock", "jazz", "blues", "techno", "rap", "hip-hop"};
    char* p;
    string sname, sdescriiption, slink, sgenres, str;

    cin.ignore();
     
    cout<<"Nume melodie: ";
    getline(cin, sname);

    cout<<"Descriere: ";
    getline(cin, sdescriiption);

    cout<<"Link: ";
    getline(cin, slink);

    
    while(q == 0)
    {
        cout<<"Genuri: ";
        getline(cin, sgenres);

        strcpy(copy, sgenres.c_str());
        p = strtok(copy, sep);
        q = 1;

        while(p != NULL)
        {
            k = 0;

            for(int i = 0; i <= 9; i++)
            {
                if(strcmp(p, knownGenres[i]) == 0)
                    k = 1;
            } 

            if(k == 0)
                {
                    cout<<"Format neacceptat pentru genuri"<<endl;
                    k = 0;
                    q = 0;
                    break;
                }

            p = strtok(NULL, sep);
        }
   
       
    }

    strcpy(name, sname.c_str());
    strcpy(description, sdescriiption.c_str());
    strcpy(link, slink.c_str());
    strcpy(genres, sgenres.c_str());

    mySend(sd, name);
    mySend(sd, description);
    mySend(sd, link);
    mySend(sd, genres);

    str = myRecv(sd);
    cout<<str<<endl;

}

void vote(int sd)
{
    char buf[256];
    string name, str;

    cin.ignore();

    cout<<"Nume: ";
    getline(cin, name);

    strcpy(buf, name.c_str());

    mySend(sd, buf);

    str = myRecv(sd);
    cout<<str<<endl;

}

void showTop(int sd)
{
    int size;
    string topLine;

    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

    for(int i = 0; i < size; i++)
    {
        topLine = myRecv(sd);
        cout<<i + 1<<". "<<topLine<<endl;
    }
}

void showTopOnGenre(int sd)
{
    int size;
    string topLine, genre;

    cout<<"Nume gen: ";
    cin.ignore();
    getline(cin, genre);

    mySend(sd, genre.c_str());

    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

    for(int i = 0; i < size; i++)
    {
        topLine = myRecv(sd);
        cout<<i + 1<<". "<<topLine<<endl;
    }
}

void getInfo(int sd)
{
    string name, description, genres, link;

    cout<<"Nume: ";
    cin.ignore();

    getline(cin, name);

    mySend(sd, name.c_str());
    description = myRecv(sd);
    genres = myRecv(sd);
    link = myRecv(sd);

    cout<<"Descriere: "<<description<<endl;
    cout<<"Genuri: "<<genres<<endl;
    cout<<"Link: "<<link<<endl;

}

void restrictVote(int sd, int opt)
{
    int size;
    string name, resp, possibleUser;

    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");
    
    if(size != -1)
    {
        for(int i = 0; i < size; i++)
        {
            possibleUser = myRecv(sd);
            cout<<i + 1<<". "<<possibleUser<<endl;
        }

        cout<<"Numele utilizatorului de modificat drepturi: ";
        cin.ignore();
        getline(cin, name);

        mySend(sd, name.c_str());
        if(-1 == send(sd, &opt, sizeof(int), 0))
            printf("%s", "Eroare la send\n");

        resp = myRecv(sd);

        cout<<resp<<endl;
    }

    else

            cout<<"Nu sunteti admin"<<endl;
          
}

void deleteSong(int sd)
{
    string name, resp;

    cout<<"Numele melodiei pe care doriti sa o stergeti: ";
    cin.ignore();
    cin>>name;

    mySend(sd, name.c_str());
    resp = myRecv(sd);
    
    cout<<resp<<endl;
    
}

void postComment(int sd)
{
    string songName, comment, resp;

    cout<<"Numele melodie la care adaugati com: ";
    cin.ignore();

    getline(cin, songName);
    cout<<"Comentariu: ";
    getline(cin, comment);

    mySend(sd, songName.c_str());
    mySend(sd, comment.c_str());

    resp = myRecv(sd);

    cout<<resp<<endl;

}

void showComments(int sd)
{
    int size;
    string songName, comment;

    cout<<"Nume melodie la care vreti sa vedeti comentarii: ";
    cin.ignore();

    getline(cin, songName);

    mySend(sd, songName.c_str());
    
    if(recv(sd, &size, sizeof(int), 0) == -1)
        printf("[Client - %d] %s", getpid(), "Eroare la recv");

    for(int i = 0; i < size; i++)
    {
        comment= myRecv(sd);
        cout<<comment<<endl;
    }
}

int main()
{
    int sd, port;
    int option, length;
    struct sockaddr_in server;
    char msg[100];
    string str;

    port = 2024;

    printf("%s\n\n", explic);

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("%s", "Erare la socket\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    if(connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1)
        printf("%s", "Eroare la conect\n");

    while(1)
    {
        while(logged == 0)
        {
            cout<<"Introdu operatia dorita: 1-logare; 2-creare user; 3-creare admin\n";
            cin>>option;

            if(-1 == send(sd, &option, sizeof(int), 0))
                printf("%s", "Eroare la send\n");

            if(option == 1)
               login(sd);

            if((option == 2) || (option == 3))
               createUser(sd);

        }


       cout<<"Introdu operatia dorita\n";
       cin>>option;

       if(-1 == send(sd, &option, sizeof(int), 0))
            printf("%s", "Eroare la send\n");

       if(option == 4)
            addSong(sd);

        if(option == 5)
            vote(sd);

        if(option == 6)
            showTop(sd);

        if(option == 7)
           showTopOnGenre(sd);

        if(option == 8)
            postComment(sd);

        if(option == 9)
           deleteSong(sd);

        if(option == 10)
            restrictVote(sd, 1);

        if(option == 11)
            getInfo(sd);

        if(option == 12)
            restrictVote(sd, 2);

        if(option == 13)
            showComments(sd);

        if(option == 14)
            logout(sd);


        if((option < 0) || (option > 14))
        {
            strcpy(msg, myRecv(sd).c_str());
            cout<<msg<<endl;

            return 0;

        }

        

    }

    close(sd);

    return 0;

}
