#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QUrl>
#include <QSoundEffect>
#include <QPainter>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <QLoggingCategory>
#include <QKeyEvent>
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <algorithm>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <cmath>
#include <QMenu>
#include <QDir>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QAction>
#include <QJsonObject>
#include <QFileDialog>
#include <QDebug>
#include <QProgressBar>
#include <typeinfo>
#include <QMap>
#include "PathPoint.h"
#include "enemy.h"
#include "tower.h"
#include "mt01.h"
#include "dtf01.h"
#include "bs01.h"
#include "bs01_death.h"

class Mainscene : public QWidget
{
    Q_OBJECT

public:
    Mainscene(QWidget *parent = nullptr);
    ~Mainscene();

    //initialize
    void initScene();
    void InitializeParameters();

    //Start the GAME
    void playGame();
    bool stop;
    bool pauseGame;

    //Update the Position
    void updatePosition();

    //Update attack
    void updateAttack();

    //increase money
    void money_count();
    int money_increase_recorder;

    //paint the event
    void paintEvent(QPaintEvent *event);

    //set map
    QPixmap m_map_1;
    QPixmap m_map_2;
    QPixmap m_map_3;
    int map_choose;

    //Identify GameState
    int GameState;

    //set Timer
    QTimer m_Timer;

    ////////////////IO control/////////////////////
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    Tower* MouseSearchForTower(int r_x,int r_y);
    Enemy* MouseSearchForEnemy(int r_x,int r_y);
    void createContextMenu();
    Enemy* currentEnemy;
    Tower* currentTower;
    QMenu *contextMenu_mt01;
    QMenu *contextMenu_dtf01;
    QMenu *contextMenu_bs01;
    QAction *addBuffSprint;
    QAction *addBuffFreeze;
    QAction *addBuffCraze;
    QAction *addBuffFast;
    QAction *consistLoseHealthAction;
    QAction *addBuffAoe;
    QAction *addBuffSpeedup;
    QAction *addBuffWiderange;
    //buff drop and buff collect
    //bool buff_fast_collect;
    //bool buff_sprint_collect;
    //bool buff_bleed_collect;
    //bool buff_freeze_collect;
    //bool buff_craze_collect;
    //bool buff_aoe_collect;
    /////////////////////////////////////////////////////////


    //Json File Part
    //read map profile
    void readJsonFile(const QString &filePath);
    //Game Save
    void saveGame(const QString &filepath);
    void loadGame(const QString &filepath);
    //Map Grid
    int map_width;
    int map_height;
    int grid_width;
    int grid_height;
    //Position Cauculation
    int Pixel_toGrid(int x);
    int Grid_toPixel(int x);
    PathPoint Distance_Estimate(int Pix_x, int Pix_y);

    //path point
    QVector<PathPoint> EnemyPath; //Enemy Route
    QVector<PathPoint> Start;
    QVector<PathPoint> End;
    //Tower Position
    QVector<PathPoint> TowerPosition;

    //////////////////Event//////////////////////////
    bool isPathValidEvent(PathPoint current,QVector<PathPoint>& visited,int depth = 0);
    void EnemyMoveEvent(Enemy& en);
    void EnemyToScene(Enemy* en);
    void TowerToScene(QVector<Tower*> tw, int x, int y);
    void WinOrLose_Detect(); //win or lose detect event
    //void MagicToScene(Magic* mg, int Pix_x, int Pix_y);
    void PutEveryEnemies();
    Enemy* SearchEnemyTarget(Tower& tw);
    QVector<Enemy*> SearchAoeTarget(Enemy* en);
    Tower* SearchTowerTarget(Enemy& en);
    double distance(int x1, int y1, int x2, int y2);

    //////////////////members////////////////////////
    //Towers
    //MT01 arrays
    Mt01 m_Mt01s[MT01_NUM];

    //Dtf01 arrays
    Dtf01 m_Dtf01s[DTF01_NUM];

    //Enemies to scene interval control
    QVector<Enemy*> KindsOfEnemy;
    QMap<Enemy*,int> Enemy_Interval_Map;

    //TowerKinds
    QVector<Tower*> KindsOfTower; //use the Tower* Vector , this avoid the vurtual problem

    //Bs01 arrays
    Bs01 m_Bs01s[BS01_NUM];
    Bs01_death m_Bs01_deaths[BS01_DEATH_NUM];

    //Death event
    void EnemyDeath_Event(Enemy* en);
    void Bs01_death_Event(int px , int py);

    //////////////////Credits///////////////////////
    // win or lose
    bool game_win;
    bool game_lose;
    // Monster Arrived
    int enemy_total;
    int enemy_left;
    int lose_condition;
    int arrived;
    // Monster destroyed
    int destroyed;
    // Monster appeared
    int monster_appeared;
    // money
    int money;

    int total_money;
    //credit buff;
    bool credit_freeze_enable;
    bool credit_craze_enable;
    void credit_apply();


    ///////////////UI/////////////////////////////

    void Initialize_UI();
    QPixmap UI_Archievement;
    QPixmap UI_ArchievementDown;
    QRect UI_Archievement_Rect;
    QPixmap UI_Credit;
    QPixmap UI_CreditDown;
    QRect UI_Credit_Rect;
    QPixmap UI_Play;
    QPixmap UI_PlayDown;
    QRect UI_Play_Rect;
    QPixmap UI_Logo;
    QPixmap UI_MainMenu;
    QPixmap UI_WaveStation;
    QRect UI_WaveStation_1_Rect;
    QRect UI_WaveStation_2_Rect;
    QRect UI_WaveStation_3_Rect;
    QPixmap UI_WaveStationLight;
    QPixmap UI_Win;
    QPixmap UI_Lose;
    QPixmap UI_Pause;
    QPixmap UI_CHOOSE_MAP;
    QPixmap UI_Heart;
    QPixmap UI_PowerBackground;
    QPixmap UI_Bottom;
    QPixmap UI_Menu;
    QPixmap UI_MenuDown;
    QRect UI_Menu_Rect;
    QPixmap UI_Update;
    QPixmap UI_UpdateDown;
    QRect UI_Update_Rect;
    QPixmap UI_Quit;
    QPixmap UI_QuitDown;
    QRect UI_Quit_Rect;
    QPixmap UI_Gold;
    QPixmap UI_EnemyLeft;
    QPixmap UI_CreditBackground;
    QPixmap UI_Credit_Freeze_Grey;
    QPixmap UI_Credit_Freeze_Normal;
    QPixmap UI_Credit_Freeze_Light;
    QRect UI_Credit_Freeze_Rect;
    QPixmap UI_Credit_Craze_Grey;
    QPixmap UI_Credit_Craze_Normal;
    QPixmap UI_Credit_Craze_Light;
    QRect UI_Credit_Craze_Rect;

    bool UI_Quit_Click;
    bool UI_Archievement_Click;
    bool UI_Update_Click;
    bool UI_Credit_Click;
    bool UI_Play_Click;
    bool UI_Menu_Click;
    bool UI_WaveStation_1_Click;
    bool UI_WaveStation_2_Click;
    bool UI_WaveStation_3_Click;
    bool UI_Credit_Freeze_Click;
    bool UI_Credit_Craze_Click;


    //////////////////////SOUND////////////////////
    //media
    void InitializeMedia();
    void mediaControl();
    QMediaPlayer MainMenuPlayer;
    QMediaPlayer PlayingGamePlayer_1;
    QMediaPlayer Player_gold;
    QMediaPlayer Player_win;
    QMediaPlayer Player_lose;

    QAudioOutput AuOut1;
    QAudioOutput AuOut2;
    QAudioOutput AuOut_gold;
    QAudioOutput AuOut_win;
    QAudioOutput AuOut_lose;



};

#endif // MAINSCENE_H
