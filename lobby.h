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
#include <QThread>

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
#define COMBO 11
#define PASS 12

QT_BEGIN_NAMESPACE
namespace Ui { class lobby; }
QT_END_NAMESPACE

class LordThread : public QThread
{
    Q_OBJECT
private:
    int lorddecided;
protected:
    void run();
public:
    LordThread(QObject *parent=0);
    ~LordThread();
signals:
    void askSignal(int);
    void playSignal();
    void lordDecideSignal(int);
public slots:
    void lordSlot();
};

class lobby : public QMainWindow
{
    Q_OBJECT

signals:
    void lordProgressSignal();

public slots:
    void acceptConnectionHost();
    void acceptConnectionMedia();
    void receiveMessage(int );
    void on_connect_clicked();

    void gameStart();
    void playStart();

    void askForLord(int);      // A 询问谁想抢地主
    void whosLord(int);        // A 告知谁是Lord ABC显示谁是LORD，卡牌是什么

    bool whetherLord();     // 决定是否叫地主
    void chooseLord(int id,bool yes);    // A 接收来自 BC的抢地主信息

    void sendComboSlot(QList<Poker>,int,int);
    void passSlot(int playid,int last_played);

    void gameEnd(int winner);

public:
    lobby(QWidget *parent = nullptr);
    ~lobby();

private:
    void sendMessageById(int,QString);
    void sendMessageByABC(int,QString);

    void playersReady();

    void decideArrange(int a,int b,int c);
    void serve();

    Ui::lobby *ui;
    game* gamewindow;
    QTcpServer* listenSocket;
    QTcpSocket* connectSocket_a, *connectSocket_b;
    QTcpSocket* readWriteSocket_b, *readWriteSocket_c;
    QTcpSocket* connectId[3];
    QTcpSocket* connectABC[3];
    QSignalMapper* mapper;
    LordThread* thread;
    int connection;
    int client_id,
        play_id,
        lord_id
    , last_played;
    int current_stage;
    QList<Poker> lordshand;
};



#endif // LOBBY_H
