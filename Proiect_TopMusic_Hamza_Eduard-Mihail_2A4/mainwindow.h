#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "MyClient.h"
#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoginButton_clicked();

    void on_registr_clicked();

    void on_regButton_clicked();

    void on_pushButton_clicked();

    void on_showTopButton_clicked();

    void on_voteButton_clicked();

    void on_pushButton_2_clicked();

    void on_goToAdauga_clicked();

    void on_addSongButton_clicked();

    void on_showInfoButton_clicked();

    void on_backVoteButton_clicked();

    void on_goToRestrPage_clicked();

    void on_restrictVote_clicked();

    void on_giveVotePriv_clicked();

    void on_deleteSongButton_clicked();

    void on_pushButton_3_clicked();

    void on_postCommentButton_clicked();

    void on_pushButton_4_clicked();

    void on_logoutButton_clicked();

    void on_openLinkButton_clicked();

private:
    Ui::MainWindow *ui;
    int sd;
    string currentSong, currentUser, currentLink="";
    MyClient c1;
};
#endif // MAINWINDOW_H
