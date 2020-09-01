#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QChar>
#include "game.h"
#include "poker.h"
#include "combo.h"
#include "check.h"
#include <algorithm>

#define WAITFORSTART 1
#define STARTUP 2
#define ARRANGE 3
#define SERVE 4
#define ASKFORLORD 5
#define WANTLORD 6
#define LORDCARDS 7
#define GAMESTART 8
#define GAMEEND 9
#define REPLAY 10

QT_BEGIN_NAMESPACE
namespace Ui { class lobby; }
QT_END_NAMESPACE

class lobby : public QMainWindow
{
    Q_OBJECT

public slots:
    void acceptConnectionHost();
    void acceptConnectionMedia();
    void receiveMessage(int );
    void on_connect_clicked();

    void gameStart();
    bool whetherLord();
    void chooseLord(int id);    // A

public:
    lobby(QWidget *parent = nullptr);
    ~lobby();

private:
    void sendMessageById(int,QString);
    void sendMessageByABC(int,QString);

    void playersReady();

    void decideArrange(int a,int b,int c);
    void serve();
    void askForLord();

    void whosLord();
    void playStart();

    Ui::lobby *ui;
    game* gamewindow;
    QTcpServer* listenSocket;
    QTcpSocket* connectSocket_a, *connectSocket_b;
    QTcpSocket* readWriteSocket_b, *readWriteSocket_c;
    QTcpSocket* connectId[3];
    QTcpSocket* connectABC[3];
    QSignalMapper* mapper;
    int connection;
    int client_id,
        play_id,
        lord_id;
    int current_stage;
};
#endif // LOBBY_H
