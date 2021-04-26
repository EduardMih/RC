#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), sd(-1)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    sd = c1.conect();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_LoginButton_clicked()
{
    string username, password;
    int resp;

    username = ui->username->text().toStdString();
    password = ui->password->text().toStdString();

    if((c1.checkString(username) == 1) && (c1.checkString(password) == 1))
    {

        cout<<username<<' '<<password<<endl;

        resp = c1.login(sd, username.c_str(), password.c_str());

        cout<<resp<<endl;

        if(resp == 1)
        {
            ui->stackedWidget->setCurrentIndex(2);
            currentUser = username;
        }

        else

            ui->loginresp->setText("Username/Parola invalide");
    }

    else

        ui->loginresp->setText("Doar litere, cifre si underscore, minim 3");

}

void MainWindow::on_registr_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_regButton_clicked()
{
    int type;
    string pass1, pass2, username, resp;

    username = ui->newUsername->text().toStdString();
    pass1 = ui->newPass1->text().toStdString();
    pass2 = ui->newPass2->text().toStdString();

    if((c1.checkString(username) == 1) && (c1.checkString(pass1) == 1) && (c1.checkString(pass2) == 1))
    {
        if(ui->userTypeBox->currentText().toStdString() == "User obisnuit")
            type = 2;

        else

            type = 3;

        if(pass1 != pass2)
            ui->regresp->setText("Parolele nu se potrivesc!");

        else

        {

            resp = c1.createUser(sd, username.c_str(), pass1.c_str(), type);
            ui->regresp->setText(QString::fromStdString(resp));

        }


          cout<<ui->userTypeBox->currentText().toStdString()<<endl;
    }

    else

        ui->regresp->setText("Doar litere, cifre, uderscore, minim 3");

}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_showTopButton_clicked()
{
    int size;
    string topLine, genre;
    vector<string> top;

    genre = ui->selectGenre->currentText().toStdString();

    if(genre == "GENERAL")
        top = c1.showTop(sd);

    else

        top = c1.showTopOnGenre(sd, genre.c_str());


    size = top.size();

    ui->topView->clear();

    for(int i = 0; i < size; i++)
    {
        topLine = to_string(i + 1) + ". " + top[i];
        ui->topView->addItem(QString::fromStdString(topLine));
    }


}

void MainWindow::on_voteButton_clicked()
{
    int pos, pos2;
    string line, song, resp;

    if(ui->topView->currentItem())
    {line = ui->topView->currentItem()->text().toStdString();
    pos = line.find(" ");
    pos2 = line.find_last_of("(");
    song = line.substr(pos + 1, pos2 - 2 - pos);

    cout<<">"<<song<<"<"<<endl;

    resp = c1.vote(sd, song.c_str());

    ui->responseLabel->setText(QString::fromStdString(resp));
    }

}


void MainWindow::on_goToAdauga_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_addSongButton_clicked()
{
    QList <QCheckBox *> checklist;
    string name, description, link, genres="", resp;

    name = ui->songName->text().toStdString();
    description = ui->description->text().toStdString();
    link = ui->link->text().toStdString();
    checklist = ui->stackedWidget->findChildren<QCheckBox *>();

    for(QCheckBox* checkbox : checklist)
        if(checkbox->isChecked())
        {
            if(genres == "")
                genres = checkbox->text().toStdString();

            else

                genres = genres + "," + checkbox->text().toStdString();

            checkbox->setCheckState(Qt::Unchecked);
        }

    if((name == "") || (description == "") || (link == "") || (genres == ""))
        ui->addSongResp->setText("Toate campurile sunt obligatorii");

    else

    {

    resp = c1.addSong(sd, name.c_str(), description.c_str(), link.c_str(), genres.c_str());

    ui->addSongResp->setText(QString::fromStdString(resp));

    }

    ui->songName->clear();
    ui->description->clear();
    ui->link->clear();

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_showInfoButton_clicked()
{
    int pos, pos2;
    string description, genres, line, song, link;
    vector<string> info;

    if(ui->topView->currentItem())
    {line = ui->topView->currentItem()->text().toStdString();
    pos = line.find(" ");
    pos2 = line.find_last_of("(");
    song = line.substr(pos + 1, pos2 - 2 - pos);
    info = c1.getSongInfo(sd, song.c_str());


    description = "<p><b>Descriere: </b>" + info[0] + "</p>";
    genres = "<p><b>Genuri: </b>" + info[1] + "</p>";
    link = "<p><b>Link: </b>" + info[2] + "</p>";
    currentLink = info[2];

    ui->textBrowser->setText(QString::fromStdString(description + genres + link));
    }

}

void MainWindow::on_backVoteButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_goToRestrPage_clicked()
{
    int size;
    vector<string> users;

    size = c1.getAdminInfo(sd);

    if(size == -1)
        ui->adminResp->setText("Nu sunteti admin!");

    else

    {

        users = c1.getUsers(sd, size);
        ui->usersList->clear();
        for(int i = 0; i < size; i++)
            ui->usersList->addItem(QString::fromStdString(users[i]));
        ui->stackedWidget->setCurrentIndex(4);

    }
}

void MainWindow::on_restrictVote_clicked()
{
    string name, resp;

    name = ui->usersList->currentItem()->text().toStdString();
    name = name.substr(0, name.find(' '));
    resp = c1.restrictVote(sd, name.c_str());

    ui->restResp->setText(QString::fromStdString(resp));
}

void MainWindow::on_giveVotePriv_clicked()
{
    string name, resp;

    name = ui->usersList->currentItem()->text().toStdString();
    name = name.substr(0, name.find(' '));

    resp = c1.giveVotePerm(sd, name.c_str());

    ui->restResp->setText(QString::fromStdString(resp));
}

void MainWindow::on_deleteSongButton_clicked()
{
    int pos, pos2;
    string line, song, resp;


    if(ui->topView->currentItem())
    {line = ui->topView->currentItem()->text().toStdString();
    pos = line.find(" ");
    pos2 = line.find_last_of("(");
    song = line.substr(pos + 1, pos2 - 2 - pos);

    resp = c1.deleteSong(sd, song.c_str());
    ui->adminResp->setText(QString::fromStdString(resp));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int pos, pos2, size;
    string line;
    vector<string> comments;

    if(ui->topView->currentItem())
    {line = ui->topView->currentItem()->text().toStdString();
    pos = line.find(" ");
    pos2 = line.find_last_of("(");
    currentSong = line.substr(pos + 1, pos2 - 2 - pos);

    comments = c1.showComments(sd, currentSong.c_str());
    size = comments.size();

    ui->commentsList->clear();
    ui->label_10->setText(QString::fromStdString(currentSong + " - Comentarii"));

    for(int i = 0; i < size; i++)
        ui->commentsList->addItem(QString::fromStdString(comments[i]));

    ui->newComment->setPlaceholderText("Scrie aici comentariul tau...");
    ui->label_11->clear();
    ui->stackedWidget->setCurrentIndex(5);
    }
}

void MainWindow::on_postCommentButton_clicked()
{
    string comment, resp;

    comment = ui->newComment->text().toStdString();

    resp = c1.postComment(sd, currentSong.c_str(), comment.c_str());

    ui->label_11->setText(QString::fromStdString(resp));
    ui->commentsList->addItem(QString::fromStdString(currentUser + ": " + comment));
    ui->newComment->clear();

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_logoutButton_clicked()
{
    c1.logout(sd);
    currentUser = "";
    ui->topView->clear();
    ui->textBrowser->clear();
    ui->username->clear();
    ui->password->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_openLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl(QString::fromStdString(currentLink)));
    currentLink = "";
}
