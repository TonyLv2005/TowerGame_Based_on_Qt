#include "mainscene.h"
#include "config.h"
#include "qapplication.h"

Mainscene::Mainscene(QWidget *parent)
    : QWidget(parent)
{
    QLoggingCategory::setFilterRules(QStringLiteral(
        "qt.multimedia.audio.debug=false\n"
        "qt.gui.imageio=false\n"
        ));

    //SetParameters();
    initScene();
    playGame();
}

Mainscene::~Mainscene()
{

}


void Mainscene::initScene()
{
    //Initialize
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    setWindowTitle(GAME_TITLE);
    createContextMenu(); //menu
    setWindowIcon(QIcon(GAME_ICON));
    InitializeMedia();
    total_money = GAME_START_MONEY;

    grid_width = 50;
    grid_height = 50;
    //mouse track enable
    setMouseTracking(true);
    money_increase_recorder = 0;
    //timer
    m_Timer.setInterval(GAME_RATE);

    //initialize parameters
    InitializeParameters();


    //Initialize GameState
    GameState = STARTSTATE ;
    //start load game
    //loadGame("save.json");

    // random seeds
    //srand((unsigned int)time(NULL));

    //Initialize UI
    Initialize_UI();

    //map initialize
    m_map_1.load(MAP_1_PATH);
    m_map_2.load(MAP_2_PATH);
    m_map_3.load(MAP_3_PATH);
    map_choose = 1;

    //bs01 _ dead
    /*
    for(int i = 1; i <= BS01_IMAGE_ATTACK_MAX ; i++){
        QString str = QString(BS01_ATTACK_PATH).arg(i,2,10,QChar('0'));
        m_Enemy_attacks.push_back(QPixmap(str));
    }*/

}

void Mainscene::InitializeParameters()
{
    stop = false;
    pauseGame = false;
    //Json
    //interval control
    Enemy_Interval_Map.clear();
    for(Enemy* KindOfEn : KindsOfEnemy){
        Enemy_Interval_Map[KindOfEn] = 0;
    }

    //reset all the instances
    for(Mt01& mt : m_Mt01s){
        mt = Mt01();
    }
    for(Dtf01& dt : m_Dtf01s){
        dt = Dtf01();
    }
    for(Bs01& bs : m_Bs01s){
        bs = Bs01();
    }
    for(Bs01_death& bs : m_Bs01_deaths){
        bs = Bs01_death();
    }

    //clear the Vector
    KindsOfEnemy.clear();
    KindsOfTower.clear();
    EnemyPath.clear();
    Start.clear();
    End.clear();
    TowerPosition.clear();



    //return to start menu
    GameState = STARTSTATE;

    //Qmenu
    currentTower = nullptr;
    currentEnemy = nullptr;
    addBuffFreeze = nullptr;
    addBuffCraze = nullptr;
    addBuffFreeze = nullptr;
    addBuffSprint = nullptr;
    consistLoseHealthAction = nullptr;

    //buff collect
    //buff_fast_collect = false;
    //buff_sprint_collect = false;
    //buff_bleed_collect = false;
    //buff_freeze_collect = false;
    //buff_craze_collect = false;
    //buff_aoe_collect = false;

    //Initialize parameters
    arrived = 0;
    destroyed = 0;
    monster_appeared = 5;

    game_win = false;
    game_lose = false;


    //credit buff init
    credit_freeze_enable = false;
    credit_craze_enable = false;
}



void Mainscene::playGame()
{
    //start the timer
    m_Timer.start();


    connect(&m_Timer,&QTimer::timeout,[=](){
        if(GameState == STARTSTATE){

        }
        if(GameState == CHOOSEMAPSTATE){

        }
        if(GameState == PLAYINGSTATE){
            if(!stop){
                PutEveryEnemies();
                updatePosition();
                updateAttack();
                WinOrLose_Detect();
                money_count();
                credit_apply();
            }
        }
        update();
        mediaControl();

        //collisionDetection();
    });
}

void Mainscene::updatePosition()
{
    //move every Bs01

    for(Bs01& obj : m_Bs01s){
        if(!obj.m_Free){
            EnemyMoveEvent(obj);
            obj.EnemyUpdate();
        }
    }

    //update bs01 death image
    for(Bs01_death& obj : m_Bs01_deaths){
        if(!obj.m_Free){
            obj.Image_update();
        }
    }

    //update every Dtf01 buff
    for(Dtf01& obj : m_Dtf01s){
        obj.TowerUpdate();
    }

    //update every Mt01/magic
    for(Mt01& obj : m_Mt01s){
        obj.TowerUpdate();
    }


}

void Mainscene::updateAttack()
{
    //every mt01 attack
    for(Mt01& obj : m_Mt01s){
        if(!obj.m_Free){
            Enemy* target = SearchEnemyTarget(obj);
            if(target != nullptr){
                obj.shoot(target);
                if(target->m_Free){
                    EnemyDeath_Event(target);
                }
            }
        }
    }

    //every dtf01 attack
    for(Dtf01& obj : m_Dtf01s){
        if(!obj.m_Free){
            Enemy* target = SearchEnemyTarget(obj);
            if(target != nullptr){
                obj.shoot(target);
                if(target->m_Free){
                    EnemyDeath_Event(target);
                }
                if(obj.aoe_available){
                QVector<Enemy*> tm = SearchAoeTarget(target);
                for(Enemy* en : tm){
                    obj.shoot(en);
                    if(en->m_Free){
                        EnemyDeath_Event(target);
                        }
                    }
                }
            }
        }
    }


    //every ds01 attack
    for(Bs01& obj : m_Bs01s){
        if(!obj.m_Free){
            Tower* target = SearchTowerTarget(obj);
            if(target != nullptr){
                //qDebug() << "ds01 try attack";
                obj.EnemyState = BS01_STATE_ATTACK;//stop and raise hand to attack // no animation now
                obj.shoot(target);
            }
            else{
                if(obj.EnemyState != BS01_STATE_DEAD){
                    obj.EnemyState = BS01_STATE_MOVE;
                }
            }
        }
    }


}

void Mainscene::money_count()
{
    if(GameState == PLAYINGSTATE){
        money_increase_recorder ++;
        if(money_increase_recorder >= MONEY_INTERVAL){
            money += MONEY_INCREASE;
            money_increase_recorder = 0;
        }

    }
}

void Mainscene::mediaControl()
{
    if(GameState == STARTSTATE){
        MainMenuPlayer.play();
        Player_win.stop();
        Player_lose.stop();
        PlayingGamePlayer_1.stop();
    }
    if(GameState == CHOOSEMAPSTATE){
        MainMenuPlayer.play();
        PlayingGamePlayer_1.stop();
    }
    if(GameState == PLAYINGSTATE){
        if(!pauseGame){
            PlayingGamePlayer_1.play();
        }
        MainMenuPlayer.stop();
    }
}

void Mainscene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //Start Game Page

    if(GameState == STARTSTATE){
        painter.drawPixmap(0,0,UI_MainMenu);
        if(UI_Archievement_Click){
            painter.drawPixmap(ARCHIEVEMENT_X,ARCHIEVEMENT_Y,UI_ArchievementDown);
        }else{
            painter.drawPixmap(ARCHIEVEMENT_X,ARCHIEVEMENT_Y,UI_Archievement);
        }
        if(UI_Credit_Click){
            painter.drawPixmap(CREDIT_X,CREDIT_Y,UI_CreditDown);
        }else{
            painter.drawPixmap(CREDIT_X,CREDIT_Y,UI_Credit);
        }
        if(UI_Play_Click){
            painter.drawPixmap(PLAY_X,PLAY_Y,UI_PlayDown);
        }else{
            painter.drawPixmap(PLAY_X,PLAY_Y,UI_Play);
        }
        if(UI_Quit_Click){
            painter.drawPixmap(QUIT_X,QUIT_Y,UI_QuitDown);
        }else{
            painter.drawPixmap(QUIT_X,QUIT_Y,UI_Quit);
        }
        painter.drawPixmap(LOGO_X,LOGO_Y,UI_Logo);
    }
    //Chosse map State

    if(GameState == CHOOSEMAPSTATE){
        painter.drawPixmap(0,0,UI_CHOOSE_MAP);

        if(UI_WaveStation_1_Click){
            painter.drawPixmap(STATION_1_X,STATION_1_Y,UI_WaveStationLight);
        }else{
            painter.drawPixmap(STATION_1_X,STATION_1_Y,UI_WaveStation);
        }
        if(UI_WaveStation_2_Click){
            painter.drawPixmap(STATION_2_X,STATION_2_Y,UI_WaveStationLight);
        }else{
            painter.drawPixmap(STATION_2_X,STATION_2_Y,UI_WaveStation);
        }
        if(UI_WaveStation_3_Click){
            painter.drawPixmap(STATION_3_X,STATION_3_Y,UI_WaveStationLight);
        }else{
            painter.drawPixmap(STATION_3_X,STATION_3_Y,UI_WaveStation);
        }
    }

    //Credit Page
    if(GameState == CREDITSTATE){
        painter.drawPixmap(0,0,UI_CreditBackground);
        painter.drawPixmap(CREDIT_GOLD_X,CREDIT_GOLD_Y,UI_Gold);
        painter.setPen(Qt::white);
        painter.drawText(CREDIT_GOLD_X + 65,CREDIT_GOLD_Y + 25,QString("%1").arg(total_money));
        //credit freeze
        if(UI_Credit_Freeze_Click){
            painter.drawPixmap(CREDIT_FREEZE_X,CREDIT_FREEZE_Y,UI_Credit_Freeze_Light);
        }else{
            if(credit_freeze_enable){
                painter.drawPixmap(CREDIT_FREEZE_X,CREDIT_FREEZE_Y,UI_Credit_Freeze_Normal);
            }
            else{
                painter.drawPixmap(CREDIT_FREEZE_X,CREDIT_FREEZE_Y,UI_Credit_Freeze_Grey);
            }
        }
        //credit craze
        if(UI_Credit_Craze_Click){
            painter.drawPixmap(CREDIT_CRAZE_X,CREDIT_CRAZE_Y,UI_Credit_Craze_Light);
        }else{
            if(credit_craze_enable){
                painter.drawPixmap(CREDIT_CRAZE_X,CREDIT_CRAZE_Y,UI_Credit_Craze_Normal);
            }
            else{
                painter.drawPixmap(CREDIT_CRAZE_X,CREDIT_CRAZE_Y,UI_Credit_Craze_Grey);
            }
        }
    }

    //MainGame
    if(GameState == PLAYINGSTATE){
        if(map_choose == 1){                            // paint the map
            painter.drawPixmap(0,0,m_map_1);
        }
        if(map_choose == 2){
            painter.drawPixmap(0,0,m_map_2);
        }
        if(map_choose == 3){
            painter.drawPixmap(0,0,m_map_3);
        }

        for(int i = 0 ; i < MT01_NUM ; i++){            //paint every Mt01
            Mt01& obj = m_Mt01s[i];
            if(!obj.m_Free){
                //qDebug() << i;
                int px = Grid_toPixel(obj.m_X) - obj.m_Rect.width()/2 - 10;
                int py = Grid_toPixel(obj.m_Y) - obj.m_Rect.height()/2 - 40;
                painter.drawPixmap(px,py,obj.m_Towers[obj.image_attack_index]);
                //

                if(obj.display_shootrange){
                    painter.setPen(Qt::black);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(Grid_toPixel(obj.m_X) - obj.shootrange, Grid_toPixel(obj.m_Y) - obj.shootrange, obj.shootrange * 2, obj.shootrange * 2);

                }


                //paint every mt01attack

                for(int j = 0 ; j < MT01_MAGIC_NUM ; j++){
                    Mt01Attack& at = obj.m_Mt01Attacks[j];
                    if(!at.m_Free){
                        //qDebug() << 111;
                        int px = at.m_Rect.x() - at.m_Rect.width()/2;
                        int py = at.m_Rect.y() - at.m_Rect.height()/2;
                        painter.drawPixmap(px,py,at.m_magics[at.image_index]);

                        /*
            painter.setPen(Qt::green);
            QString pointText = QString("(%1, %2)").arg(obj.m_X).arg(obj.m_Y);
            painter.drawText(px,py,pointText);
           */

                    }
                }

                //paint the bar
                int barX = obj.m_Rect.x() - obj.m_Rect.width()/4 - 10;
                int barY = py + 20;

                painter.setPen(Qt::NoPen);
                painter.setBrush(Qt::red);
                painter.drawRect(barX, barY, obj.barWidth, obj.barHeight);


                double healthPercentage = double(obj.hp) / obj.max_hp;
                painter.setBrush(Qt::green);
                painter.drawRect(barX, barY, obj.barWidth * healthPercentage, obj.barHeight);


                //paint the buff
                int buff_x;
                int buff_y;
                buff_x = Grid_toPixel(obj.m_X) - 30;
                buff_y = Grid_toPixel(obj.m_Y);
                painter.setPen(Qt::white);

                if(obj.buff_freeze_collect){
                    QString pointText = QString("Freeze");
                    if(obj.freeze_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;

                if(obj.buff_bleed_collect){
                    if(obj.consist_damage_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("Bleed");
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;

                if(obj.buff_widerange_collect){
                    if(obj.widerange_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("WideRange");
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;

                if(obj.buff_speedup_collect){
                    if(obj.speedup_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("Speed");
                    painter.drawText(buff_x,buff_y,pointText);
                }

            }
        }

        //paint every dtf01
        for(int i = 0 ; i < DTF01_NUM ; i++){
            Dtf01& obj = m_Dtf01s[i];
            if(!obj.m_Free){
                int px = Grid_toPixel(obj.m_X);// - obj.m_Rect.width()/2 - 10;
                int py = Grid_toPixel(obj.m_Y);// - obj.m_Rect.height()/2 - 40;
                painter.drawPixmap(px,py,obj.m_Towers[obj.image_attack_index]);

                //paint the bar
                int barX = obj.m_Rect.x() - obj.m_Rect.width()/4;
                int barY = py - 5;

                painter.setPen(Qt::NoPen);
                painter.setBrush(Qt::red);
                painter.drawRect(barX, barY, obj.barWidth, obj.barHeight);

                double healthPercentage = double(obj.hp) / obj.max_hp;
                painter.setBrush(Qt::green);
                painter.drawRect(barX, barY, obj.barWidth * healthPercentage, obj.barHeight);

                //paint the buff
                int buff_x;
                int buff_y;
                buff_x = Grid_toPixel(obj.m_X) - 10;
                buff_y = Grid_toPixel(obj.m_Y) + 25;
                painter.setPen(Qt::white);

                if(obj.buff_freeze_collect){
                    QString pointText = QString("Freeze");
                    if(obj.freeze_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;

                if(obj.buff_craze_collect){
                    if(obj.craze_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("Craze");
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;

                if(obj.buff_aoe_collect){
                    if(obj.aoe_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("AOE");
                    painter.drawText(buff_x,buff_y,pointText);
                }

            }
        }



        //paint every Bs01


        for(int i = 0 ; i < BS01_NUM ; i++){
            Bs01& obj = m_Bs01s[i];
            if(!obj.m_Free){
                int px = obj.m_Rect.x() - obj.m_Rect.width()/2;
                int py = obj.m_Rect.y() - obj.m_Rect.height()/2;
                painter.drawPixmap(px,py,obj.m_Enemys[obj.image_index]);
                // paint their cordinate
                painter.setPen(Qt::green);
                QString pointText = QString("(%1, %2)").arg(obj.m_X).arg(obj.m_Y);
                painter.drawText(px,py,pointText);

                //paint the bar
                int barX = obj.m_Rect.x() - obj.m_Rect.width()/4;
                int barY = py - 5;

                painter.setPen(Qt::NoPen);
                painter.setBrush(Qt::red);
                painter.drawRect(barX, barY, obj.barWidth, obj.barHeight);


                double healthPercentage = double(obj.hp) / obj.max_hp;
                painter.setBrush(Qt::green);
                painter.drawRect(barX, barY, obj.barWidth * healthPercentage, obj.barHeight);

                //paint the buff
                int buff_x;
                int buff_y;
                buff_x = obj.m_Rect.x();
                buff_y = obj.m_Rect.y() + 20;
                painter.setPen(Qt::white);

                if(obj.buff_sprint_collect){
                    if(obj.sprint_available){
                        painter.setPen(Qt::red);
                    }else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("Sprint");
                    painter.drawText(buff_x,buff_y,pointText);
                }
                buff_y += 10;
                if(obj.buff_fast_collect){
                    if(obj.fast_available){
                        painter.setPen(Qt::red);
                    }
                    else{
                        painter.setPen(Qt::white);
                    }
                    QString pointText = QString("Fast");
                    painter.drawText(buff_x,buff_y,pointText);
                }
            }
        }

        //paint every bs01_death
        for(int i = 0 ; i < BS01_DEATH_NUM; i++){
            Bs01_death& obj = m_Bs01_deaths[i];
            if(!obj.m_Free){
                int px = obj.pix_x - m_Bs01s[0].m_Rect.width()/2;
                int py = obj.pix_y - m_Bs01s[0].m_Rect.height()/2;

                painter.drawPixmap(px,py,obj.m_images[obj.image_index]);
            }
        }

        //paint the cordinate
        /*
        painter.setPen(Qt::blue);
        for (const PathPoint &point : EnemyPath) {
            QString pointText = QString("(%1, %2)").arg(point.x).arg(point.y);
            painter.drawText((point.x - 0) * grid_width, (point.y - 0) * grid_height, pointText);
        }
        painter.setPen(Qt::red);
        for (const PathPoint &point : Start) {
            QString pointText = QString("(%1, %2)").arg(point.x).arg(point.y);
            painter.drawText((point.x - 0) * grid_width, (point.y - 0) * grid_height, pointText);
        }
        for (const PathPoint &point : End) {
            QString pointText = QString("(%1, %2)").arg(point.x).arg(point.y);
            painter.drawText((point.x - 0) * grid_width, (point.y - 0) * grid_height, pointText);
        }
        painter.setPen(Qt::black);
        for (const PathPoint &point : TowerPosition) {
            QString pointText = QString("(%1, %2)").arg(point.x).arg(point.y);
            painter.drawText((point.x - 0) * grid_width, (point.y - 0) * grid_height, pointText);
        }
        */

        /////////////////////UI/////////////////////////////

        painter.drawPixmap(0,580,UI_Bottom);
        painter.drawPixmap(50,600,UI_Gold);
        painter.drawPixmap(800,580,UI_EnemyLeft);
        painter.setPen(Qt::white);
        //painter.setFont()
        painter.drawText(140,625,QString("%1").arg(money));
        painter.drawPixmap(250,602,UI_Heart);
        painter.drawText(350,625,QString("%1").arg(lose_condition - arrived));
        painter.drawText(880,625,QString("%1").arg(enemy_left));

        //paint win/lose page
        if(game_win || game_lose || pauseGame){
            if(game_win){
                painter.drawPixmap(GAME_WIDTH/2 - UI_Win.width()/2, GAME_HEIGHT/2 - UI_Win.height()/2, UI_Win);
            }
            if(game_lose){
                painter.drawPixmap(GAME_WIDTH/2 - UI_Lose.width()/2, GAME_HEIGHT/2 - UI_Lose.height()/2, UI_Lose);
            }
            if(pauseGame){
                painter.drawPixmap(GAME_WIDTH/2 - UI_Pause.width()/2, GAME_HEIGHT/2 - UI_Pause.height()/2 , UI_Pause);
                if(UI_Update_Click){
                    painter.drawPixmap(UPDATE_X, UPDATE_Y ,UI_UpdateDown);
                }else{
                    painter.drawPixmap(UPDATE_X, UPDATE_Y ,UI_Update);
                }
            }
            painter.drawPixmap(MENU_X, MENU_Y ,UI_Menu);
            if(UI_Menu_Click){
                painter.drawPixmap(MENU_X, MENU_Y ,UI_MenuDown);
            }else{
                painter.drawPixmap(MENU_X, MENU_Y ,UI_Menu);
            }
        }
        //paint the buff settings:
        //just for over all instances
        /*
    int initial_pix_x = 5;
    int initial_pix_y = 200;
    painter.setPen(Qt::white);
    if(buff_bleed_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "Bleed : GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "Bleed : NOPE");
    }
    initial_pix_y += 20;
    if(buff_freeze_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "Freeze : GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "Freeze : NOPE");
    }
    initial_pix_y += 20;
    if(buff_craze_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "Craze: GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "Craze : NOPE");
    }
    initial_pix_y += 20;
    if(buff_fast_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "Fast: GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "Fast : NOPE");
    }
    initial_pix_y += 20;
    if(buff_sprint_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "Sprint: GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "Sprint : NOPE");
    }
    initial_pix_y += 20;
    if(buff_aoe_collect){
        painter.drawText(initial_pix_x,initial_pix_y, "AOE: GET");
    }else{
        painter.drawText(initial_pix_x,initial_pix_y, "AOE : NOPE");
    }
    */

    }



}

void Mainscene::mouseMoveEvent(QMouseEvent *event)
{
    QPointF mouse = event->position();
    int mouse_X = static_cast<int>(mouse.x());
    int mouse_Y = static_cast<int>(mouse.y());

    if(GameState == PLAYINGSTATE){
        PathPoint result = Distance_Estimate(mouse_X,mouse_Y);
        //search for mt01
        for(Mt01& mt : m_Mt01s){
            if(!mt.m_Free && mt.m_X == result.x && mt.m_Y == result.y){
                mt.display_shootrange = true;
            }else{
                mt.display_shootrange = false;
            }
        }
        //search dtf01
        // >>>>>>........
    }
}

void Mainscene::mousePressEvent(QMouseEvent *event)
{
    QPointF mouse = event->position();
    int mouse_X = static_cast<int>(mouse.x());
    int mouse_Y = static_cast<int>(mouse.y());

    if(event->button() == Qt::LeftButton){
        //start
        if(GameState == STARTSTATE){
            if(UI_Play_Rect.contains(mouse_X,mouse_Y)){
                UI_Play_Click = true;

                QTimer::singleShot(100, this, [this]() {
                    UI_Play_Click = false;
                });

                QTimer::singleShot(300, this, [this]() {
                    this->GameState = CHOOSEMAPSTATE;
                });
            }
            if(UI_Credit_Rect.contains(mouse_X,mouse_Y)){
                UI_Credit_Click = true;

                QTimer::singleShot(100, this, [this]() {
                    UI_Credit_Click = false;
                });

                QTimer::singleShot(300, this, [this]() {
                    this->GameState = CREDITSTATE;
                });
            }
            if(UI_Archievement_Rect.contains(mouse_X,mouse_Y)){
                UI_Archievement_Click = true;

                QTimer::singleShot(100, this, [this]() {
                    UI_Archievement_Click = false;
                });

                QTimer::singleShot(300, this, [this]() {
                    QString filePath = QFileDialog::getOpenFileName(this,tr("Open Save File"),"",tr("Save Files (*.json);;All Files (*)"));

                    if(!filePath.isEmpty()){
                        //save_path = filePath;
                        loadGame(filePath);
                        //get the map_choose
                        if(map_choose == 1){
                            readJsonFile(MAP1_JSON);
                        }
                        if(map_choose == 2){
                            readJsonFile(MAP2_JSON);
                        }
                        if(map_choose == 3){
                            readJsonFile(MAP3_JSON);
                        }
                        //need to read json manually because the Points Path are't saved
                        //now reload the game save to change enmey_left .......

                        loadGame(filePath);
                        GameState = PLAYINGSTATE;
                    }
                });
            }
            if(UI_Quit_Rect.contains(mouse_X,mouse_Y)){
                UI_Quit_Click = true;

                QTimer::singleShot(100, this, [this]() {
                    UI_Quit_Click = false;
                });

                QTimer::singleShot(300, this, [this]() {
                    QApplication::quit();
                });
            }
        }
        //credit
        if(GameState == CREDITSTATE){
            if(UI_Credit_Freeze_Rect.contains(mouse_X,mouse_Y)){
                UI_Credit_Freeze_Click = true;
                if(total_money >= CREDIT_FREEZE_COST && !credit_freeze_enable){
                    Player_gold.play();
                    total_money -= CREDIT_FREEZE_COST;
                    credit_freeze_enable = true;
                }

                QTimer::singleShot(100, this, [this]() {
                    UI_Credit_Freeze_Click = false;
                });
            }

            if(UI_Credit_Craze_Rect.contains(mouse_X,mouse_Y)){
                UI_Credit_Craze_Click = true;

                if(total_money >= CREDIT_CRAZE_COST && !credit_craze_enable){
                    Player_gold.play();
                    total_money -= CREDIT_CRAZE_COST;
                    credit_craze_enable = true;
                }

                QTimer::singleShot(100, this, [this]() {
                    UI_Credit_Craze_Click = false;
                });
            }
        }

        //Choose
        if(GameState == CHOOSEMAPSTATE){
            if(UI_WaveStation_1_Rect.contains(mouse_X,mouse_Y)){
                UI_WaveStation_1_Click = true;

                QTimer::singleShot(300, this, [this]() {
                    GameState = PLAYINGSTATE;
                    readJsonFile(MAP1_JSON);
                    map_choose = 1;
                });

                QTimer::singleShot(100, this, [this]() {
                    UI_WaveStation_1_Click = false;
                });
            }
            if(UI_WaveStation_2_Rect.contains(mouse_X,mouse_Y)){
                UI_WaveStation_2_Click = true;

                QTimer::singleShot(300, this, [this]() {
                    GameState = PLAYINGSTATE;
                    readJsonFile(MAP2_JSON);
                    map_choose = 2;
                });

                QTimer::singleShot(100, this, [this]() {
                    UI_WaveStation_2_Click = false;
                });
            }
            if(UI_WaveStation_3_Rect.contains(mouse_X,mouse_Y)){
                UI_WaveStation_3_Click = true;

                QTimer::singleShot(300, this, [this]() {
                    GameState = PLAYINGSTATE;
                    readJsonFile(MAP3_JSON);
                    map_choose = 3;
                });

                QTimer::singleShot(100, this, [this]() {
                    UI_WaveStation_3_Click = false;
                });
            }
        }
        //mainstage
        if(GameState == PLAYINGSTATE){
            if(game_win || game_lose){
                if(UI_Menu_Rect.contains(mouse_X,mouse_Y)){
                    UI_Menu_Click = true;

                    QTimer::singleShot(100, this, [this]() {
                        UI_Menu_Click = false;
                    });

                    QTimer::singleShot(300, this, [this]() {
                        this->InitializeParameters();
                    });
                }
            }
            if(pauseGame){
                if(UI_Update_Rect.contains(mouse_X,mouse_Y)){
                    UI_Update_Click = true;

                    QTimer::singleShot(100, this, [this]() {
                        UI_Update_Click = false;
                    });

                    QTimer::singleShot(300, this, [this]() {
                    QString filePath = QFileDialog::getOpenFileName(this,tr("Save to"),"",tr("Save Files (*.json);;All Files (*)"));

                    if(!filePath.isEmpty()){
                        saveGame(filePath);
                    }
                    });
                }

                if(UI_Menu_Rect.contains(mouse_X,mouse_Y)){
                    UI_Menu_Click = true;

                    QTimer::singleShot(100, this, [this]() {
                        UI_Menu_Click = false;
                    });

                    QTimer::singleShot(300, this, [this]() {
                        this->InitializeParameters();
                    });
                }
            }

                PathPoint result = Distance_Estimate(mouse_X,mouse_Y);
                //test whether the Towerposition is valid
                for(PathPoint pt : TowerPosition){
                    if(pt.x == result.x && pt.y == result.y){
                        QVector<Tower*> temp = {};
                        for(int i = 0; i< m_Mt01s[0].getChildStatic(); i ++){
                        temp.push_back(&m_Mt01s[i]);
                        }
                        TowerToScene(temp,result.x,result.y);
                        break;
                    }
                }

                for(PathPoint pt : EnemyPath){
                    if(pt.x == result.x && pt.y == result.y){
                        QVector<Tower*> temp = {};
                        for(int i = 0; i< m_Dtf01s[0].getChildStatic(); i ++){
                        temp.push_back(&m_Dtf01s[i]);
                        }
                        TowerToScene(temp,result.x,result.y);
                        break;
                    }
                }


        }
    }

    if(event->button() == Qt::RightButton){

        if(GameState == PLAYINGSTATE){
            PathPoint result = Distance_Estimate(mouse_X,mouse_Y);
            Tower* tw = MouseSearchForTower(result.x,result.y);
            Enemy* en = MouseSearchForEnemy(result.x,result.y);
            if(tw != nullptr){
                if(tw->Identity == 0){
                    currentTower = tw;
                    contextMenu_mt01->exec(event->globalPosition().toPoint());
                }
                if(tw->Identity == 1){
                    currentTower = tw;
                    contextMenu_dtf01->exec(event->globalPosition().toPoint());
                }
            }
            if(en != nullptr){
                if(en->Identity == 1){
                    currentEnemy = en;
                    contextMenu_bs01->exec(event->globalPosition().toPoint());
                }
            }
        }
    }

}

void Mainscene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        if(GameState == CHOOSEMAPSTATE){
            GameState = STARTSTATE;
        }
        if(GameState == CREDITSTATE){
            GameState = STARTSTATE;
        }
        if(GameState == PLAYINGSTATE){
            if(pauseGame){
                PlayingGamePlayer_1.play();
                pauseGame = false;
                stop = false;
            }else{
                PlayingGamePlayer_1.pause();
                pauseGame = true;
                stop = true;
            }
        }

    }
}

Tower* Mainscene::MouseSearchForTower(int r_x, int r_y)
{
    for(Mt01& obj : m_Mt01s){
        if(obj.m_Free){
            continue;
        }
        if(obj.m_X == r_x && obj.m_Y == r_y){
            return &obj;
        }
        }


    for(Dtf01& obj : m_Dtf01s){
        if(obj.m_Free){
            continue;
        }
        if(obj.m_X == r_x && obj.m_Y == r_y){
            return &obj;
        }
        }
    //hardcode
/*
    for(Tower* KindOfTw : KindsOfTower){
        Tower* obj = nullptr;
        for(int i = 0; i < KindOfTw->getChildStatic() ; i++){
            obj = &KindOfTw[i];
            if(obj->m_Free){
                continue;
            }
            if(obj->m_X == r_x && obj->m_Y == r_y){
                return obj;
            }
        }
    }
*/
        return nullptr;
}

Enemy *Mainscene::MouseSearchForEnemy(int r_x, int r_y)
{
        for(Bs01& obj : m_Bs01s){
            if(obj.m_Free){
                continue;
            }
            if(obj.m_X == r_x && obj.m_Y == r_y){
                return &obj;
            }
        }
        //hardcode
        return nullptr;
}

void Mainscene::createContextMenu()
{
    contextMenu_dtf01 = new QMenu(this);
    contextMenu_mt01 = new QMenu(this);
    contextMenu_bs01 = new QMenu(this);

    //add freeze
    addBuffFreeze = new QAction("Freeze", this);
    connect(addBuffFreeze, &QAction::triggered, this, [this](){
        if(currentTower){
            if(currentTower->buff_freeze_collect){
                if(currentTower->freeze_available){
                    currentTower->freeze_available = false;
                }else{
                    currentTower->freeze_available = true;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "freeze has not been collected";
            }
        }
    });
    contextMenu_dtf01->addAction(addBuffFreeze);
    contextMenu_mt01->addAction(addBuffFreeze);

    //add lose health
    consistLoseHealthAction = new QAction("Bleed", this);
    connect(consistLoseHealthAction, &QAction::triggered, [this]() {
        if (currentTower) {
            if(currentTower->buff_bleed_collect){
                if(currentTower->consist_damage_available){
                    currentTower->consist_damage_available = false;
                }else{
                    currentTower->consist_damage_available = true;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "Bleed has not been collected";
            }
        }
    });
    contextMenu_mt01->addAction(consistLoseHealthAction);

    //speedup for mt01

    addBuffSpeedup = new QAction("Speedup", this);
    connect(addBuffSpeedup, &QAction::triggered, [this]() {
        if (currentTower) {
            if(currentTower->buff_speedup_collect){
                if(currentTower->speedup_available){
                    currentTower->speedup_available = false;
                    currentTower->m_interval = currentTower->max_interval;
                }else{
                    currentTower->speedup_available = true;
                    currentTower->m_interval = currentTower->max_interval/2;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "Speedup has not been collected";
            }
        }
    });

    contextMenu_mt01->addAction(addBuffSpeedup);

    //wide range for mt01

    addBuffWiderange = new QAction("WideRange", this);
    connect(addBuffWiderange, &QAction::triggered, [this]() {
        if (currentTower) {
            if(currentTower->buff_widerange_collect){
                if(currentTower->widerange_available){
                    currentTower->widerange_available = false;
                }else{
                    currentTower->widerange_available = true;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "WideRange has not been collected";
            }
        }
    });

    contextMenu_mt01->addAction(addBuffWiderange);



    //add craze
    addBuffCraze = new QAction("Craze", this);
    connect(addBuffCraze, &QAction::triggered, [this]() {
        if (currentTower) {
            if(currentTower->buff_craze_collect){
                if(currentTower->craze_available){
                    currentTower->craze_available = false;
                }else{
                    currentTower->craze_available = true;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "Craze has not been collected";
            }
        }
    });
    contextMenu_dtf01->addAction(addBuffCraze);

    //add aoe
    addBuffAoe = new QAction("Aoe", this);
    connect(addBuffAoe, &QAction::triggered, [this]() {
        if (currentTower) {
            if(currentTower->buff_aoe_collect){
                if(currentTower->aoe_available){
                    currentTower->aoe_available = false;
                }else{
                    currentTower->aoe_available = true;
                    currentTower->Player_buff->play();
                }
            }
            else{
                qDebug() << "Aoe has not been collected";
            }

        }
    });
    contextMenu_dtf01->addAction(addBuffAoe);
    //add fast
    /*
    addBuffFast = new QAction("Fast", this);
    connect(addBuffFast, &QAction::triggered, [this]() {
        if (currentEnemy) {
            if(currentEnemy->buff_fast_collect){
                currentEnemy->addBuff_fast();
            }
            else{
                qDebug()<< "Fast has not been collected";
            }

        }
    });

    contextMenu_bs01->addAction(addBuffFast);
    */
    //add fast sprint
    addBuffSprint = new QAction("Sprint", this);
    connect(addBuffSprint, &QAction::triggered, [this]() {
        if (currentEnemy) {
            if(currentEnemy->buff_sprint_collect){
                currentEnemy->addBuff_Sprint();
            }
            else{
                qDebug() << "sprint has not been collected";
                currentTower->Player_buff->play();
            }
        }
    });

    contextMenu_bs01->addAction(addBuffSprint);
}


void Mainscene::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);

    //get the map grid's width and height
    QJsonObject jsonObj = doc.object();
    QJsonObject mapSize = jsonObj["mapSize"].toObject();
    map_width = mapSize["width"].toInt();
    map_height = mapSize["height"].toInt();

    QJsonObject game = jsonObj["game"].toObject();
    enemy_total = game["enemy_left"].toInt();
    enemy_left = enemy_total;
    lose_condition = game["lose_condition"].toInt();
    money = game["money"].toInt();


    //get the given available path
    QJsonObject paths = jsonObj["paths"].toObject();

    //start point
    QJsonArray startPoints = paths["start"].toArray();
    for(const QJsonValue &startValue : startPoints){
        QJsonObject point = startValue.toObject();
        PathPoint temp(point["x"].toInt(),point["y"].toInt());
        Start.push_back(temp);
    }

    //end point
    QJsonArray endPoints = paths["end"].toArray();
    for(const QJsonValue &endValue : endPoints){
        QJsonObject point = endValue.toObject();
        PathPoint temp(point["x"].toInt(),point["y"].toInt());
        End.push_back(temp);
    }
    //path points
    QJsonArray points = paths["points"].toArray();
    for(const QJsonValue &pointValue : points){
        QJsonObject point = pointValue.toObject();
        PathPoint temp(point["x"].toInt(),point["y"].toInt());
        EnemyPath.push_back(temp);
    }

    //Tower position
    QJsonArray TP = jsonObj["TowerPosition"].toArray();
    for(const QJsonValue &TowerValue : TP){
        QJsonObject point = TowerValue.toObject();
        PathPoint temp(point["x"].toInt(),point["y"].toInt());
        TowerPosition.push_back(temp);
    }

    //Get Kinds Of Enemy
    KindsOfEnemy.push_back(m_Bs01s);

    //Get Kinds Of Tower
    KindsOfTower.push_back(m_Dtf01s);
    KindsOfTower.push_back(m_Mt01s);


    //Yet to be realized
}

void Mainscene::saveGame(const QString &filePath)
{
    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject saveData;
    QJsonArray towersArray;

    for (const Mt01& tower : m_Mt01s) {
        QJsonObject towerObject;
        towerObject["type"] = "Mt01";
        towerObject["x"] = tower.m_X;
        towerObject["y"] = tower.m_Y;
        towerObject["hp"] = tower.hp;
        towerObject["freeze_available"] = tower.freeze_available;
        towerObject["consist_damage_available"] = tower.consist_damage_available;
        towerObject["craze_available"] = tower.craze_available;
        towerObject["m_recorder"] = tower.m_recorder;
        towerObject["m_interval"] = tower.m_interval;
        towerObject["m_Free"] = tower.m_Free;
        towerObject["m_Rect_x"] = tower.m_Rect.x();
        towerObject["m_Rect_y"] = tower.m_Rect.y();
        towerObject["damage"] = tower.damage;
        towerObject["shootrange"] = tower.shootrange;

        QJsonArray attacksArray;
        for (const Mt01Attack& attack : tower.m_Mt01Attacks) {
                QJsonObject attackObject;
                attackObject["m_X"] = attack.m_X;
                attackObject["m_Y"] = attack.m_Y;
                attackObject["m_Rect_x"] = attack.m_Rect.x();
                attackObject["m_Rect_y"] = attack.m_Rect.y();
                attackObject["m_Free"] = attack.m_Free;
                attacksArray.append(attackObject);
        }
        towerObject["attacks"] = attacksArray;

        towersArray.append(towerObject);
    }

    for (const Dtf01& tower : m_Dtf01s) {
        QJsonObject towerObject;
        towerObject["type"] = "Dtf01";
        towerObject["x"] = tower.m_X;
        towerObject["y"] = tower.m_Y;
        towerObject["hp"] = tower.hp;
        towerObject["freeze_available"] = tower.freeze_available;
        towerObject["consist_damage_available"] = tower.consist_damage_available;
        towerObject["craze_available"] = tower.craze_available;
        towerObject["m_recorder"] = tower.m_recorder;
        towerObject["m_interval"] = tower.m_interval;
        towerObject["m_Rect_x"] = tower.m_Rect.x();
        towerObject["m_Rect_y"] = tower.m_Rect.y();
        towerObject["m_Free"] = tower.m_Free;
        towerObject["damage"] = tower.damage;
        towerObject["shootrange"] = tower.shootrange;
        //towerObject["aoe_available"] = tower.aoe_available; //tobedone
        towersArray.append(towerObject);
    }

    saveData["towers"] = towersArray;

    QJsonArray enemiesArray;
    for (const Bs01& enemy : m_Bs01s) {
        QJsonObject enemyObject;
        enemyObject["type"] = "Bs01";
        enemyObject["m_Rect_x"] = enemy.m_Rect.x();
        enemyObject["m_Rect_y"] = enemy.m_Rect.y();
        enemyObject["x"] = enemy.m_X;
        enemyObject["y"] = enemy.m_Y;
        enemyObject["hp"] = enemy.hp;
        enemyObject["m_damage"] = enemy.m_damage;
        enemyObject["m_speed"] = enemy.m_speed;
        enemyObject["m_Free"] = enemy.m_Free;
        enemyObject["shootrange"] = enemy.shootrange;
        enemyObject["m_interval"] = enemy.m_interval;
        enemyObject["m_recorder"] = enemy.m_recorder;
        enemyObject["m_consist_losehealth"] = enemy.m_consist_losehealth;
        enemyObject["m_consist_recorder"] = enemy.m_consist_recorder;
        enemyObject["m_consist_total"] = enemy.m_consist_total;
        enemyObject["m_freeze"] = enemy.m_freeze;
        enemyObject["m_freeze_recorder"] = enemy.m_freeze_recorder;
        enemyObject["fast_available"] = enemy.fast_available;
        enemyObject["sprint_available"] = enemy.sprint_available;
        enemyObject["m_sprint_recorder"] = enemy.m_sprint_recorder;
        enemyObject["m_sprint"] = enemy.m_sprint;

        // Save HaveDone and target
        QJsonArray haveDoneArray;
        for (const PathPoint& point : enemy.HaveDone) {
                QJsonObject pointObject;
                pointObject["x"] = point.x;
                pointObject["y"] = point.y;
                haveDoneArray.append(pointObject);
        }
        enemyObject["HaveDone"] = haveDoneArray;

        QJsonObject targetObject;
        targetObject["x"] = enemy.target.x;
        targetObject["y"] = enemy.target.y;
        enemyObject["target"] = targetObject;

        enemiesArray.append(enemyObject);
    }

    saveData["enemies"] = enemiesArray;

    QJsonObject gameStateObject;
    gameStateObject["enemy_left"] = enemy_left;
    gameStateObject["arrived"] = arrived;
    gameStateObject["destroyed"] = destroyed;
    gameStateObject["money"] = money;
    gameStateObject["game_win"] = game_win;
    gameStateObject["game_lose"] = game_lose;
    gameStateObject["monster_appeared"] = monster_appeared;
    //
    gameStateObject["map_choose"] = map_choose;
    gameStateObject["enemy_total"] = enemy_total;
    gameStateObject["lose_condition"] = lose_condition;


    saveData["game_state"] = gameStateObject;

    QJsonDocument saveDoc(saveData);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void Mainscene::loadGame(const QString &filePath)
{
    QFile loadFile(filePath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    int mt01 = 0;
    int dtf01 = 0;
    int bs01 = 0;

    // Load towers
    QJsonArray towersArray = json["towers"].toArray();
    for (QJsonValue towerValue : towersArray) {
        QJsonObject towerObject = towerValue.toObject();
        QString type = towerObject["type"].toString();
        int x = towerObject["x"].toInt();
        int y = towerObject["y"].toInt();
        int hp = towerObject["hp"].toInt();
        bool freeze_available = towerObject["freeze_available"].toBool();
        bool consist_damage_available = towerObject["consist_damage_available"].toBool();
        bool craze_available = towerObject["craze_available"].toBool();
        int m_recorder = towerObject["m_recorder"].toInt();
        int m_interval = towerObject["m_interval"].toInt();
        bool m_Free = towerObject["m_Free"].toBool();
        int m_Rect_x = towerObject["m_Rect_x"].toInt();
        int m_Rect_y = towerObject["m_Rect_y"].toInt();
        int damage = towerObject["damage"].toInt();
        int shootrange = towerObject["shootrange"].toInt();

        if (type == "Mt01") {
                Mt01& tower = m_Mt01s[mt01];
                tower.m_Free = m_Free;
                tower.m_X = x;
                tower.m_Y = y;
                tower.hp = hp;
                tower.freeze_available = freeze_available;
                tower.consist_damage_available = consist_damage_available;
                tower.craze_available = craze_available;
                tower.m_recorder = m_recorder;
                tower.m_interval = m_interval;
                tower.damage = damage;
                tower.shootrange = shootrange;
                tower.m_Rect.moveTo(m_Rect_x, m_Rect_y);

                // Load Mt01Attack for this tower
                QJsonArray attacksArray = towerObject["attacks"].toArray();
                for (int i = 0; i < attacksArray.size(); ++i) {
                QJsonObject attackObject = attacksArray[i].toObject();
                Mt01Attack& attack = tower.m_Mt01Attacks[i];
                attack.m_X = attackObject["m_X"].toInt();
                attack.m_Y = attackObject["m_Y"].toInt();
                attack.m_Rect.moveTo(attackObject["m_Rect_x"].toInt(), attackObject["m_Rect_y"].toInt());
                attack.m_Free = attackObject["m_Free"].toBool();
                }

                mt01++;
        } else if (type == "Dtf01") {
                Dtf01& tower = m_Dtf01s[dtf01];
                tower.m_Free = m_Free;
                tower.m_X = x;
                tower.m_Y = y;
                tower.hp = hp;
                tower.freeze_available = freeze_available;
                tower.consist_damage_available = consist_damage_available;
                tower.craze_available = craze_available;
                tower.m_recorder = m_recorder;
                tower.m_interval = m_interval;
                tower.damage = damage;
                tower.shootrange = shootrange;
                tower.m_Rect.moveTo(m_Rect_x, m_Rect_y);
                dtf01++;
        }
    }

    // Load enemies
    QJsonArray enemiesArray = json["enemies"].toArray();
    for (QJsonValue enemyValue : enemiesArray) {
        QJsonObject enemyObject = enemyValue.toObject();
        QString type = enemyObject["type"].toString();
        int x = enemyObject["x"].toInt();
        int y = enemyObject["y"].toInt();
        int hp = enemyObject["hp"].toInt();
        int m_damage = enemyObject["m_damage"].toInt();
        int m_speed = enemyObject["m_speed"].toInt();
        bool m_Free = enemyObject["m_Free"].toBool();
        int m_Rect_x = enemyObject["m_Rect_x"].toInt();
        int m_Rect_y = enemyObject["m_Rect_y"].toInt();
        int shootrange = enemyObject["shootrange"].toInt();
        int m_interval = enemyObject["m_interval"].toInt();
        int m_recorder = enemyObject["m_recorder"].toInt();
        bool m_consist_losehealth = enemyObject["m_consist_losehealth"].toBool();
        int m_consist_recorder = enemyObject["m_consist_recorder"].toInt();
        int m_consist_total = enemyObject["m_consist_total"].toInt();
        bool m_freeze = enemyObject["m_freeze"].toBool();
        int m_freeze_recorder = enemyObject["m_freeze_recorder"].toInt();
        bool fast_available = enemyObject["fast_available"].toBool();
        bool sprint_available = enemyObject["sprint_available"].toBool();
        int m_sprint_recorder = enemyObject["m_sprint_recorder"].toInt();
        bool m_sprint = enemyObject["m_sprint"].toBool();

        if (type == "Bs01") {
                Bs01& enemy = m_Bs01s[bs01];
                enemy.m_Free = m_Free;
                enemy.m_X = x;
                enemy.m_Y = y;
                enemy.hp = hp;
                enemy.m_damage = m_damage;
                enemy.m_speed = m_speed;
                enemy.shootrange = shootrange;
                enemy.m_interval = m_interval;
                enemy.m_recorder = m_recorder;
                enemy.m_consist_losehealth = m_consist_losehealth;
                enemy.m_consist_recorder = m_consist_recorder;
                enemy.m_consist_total = m_consist_total;
                enemy.m_freeze = m_freeze;
                enemy.m_freeze_recorder = m_freeze_recorder;
                enemy.fast_available = fast_available;
                enemy.sprint_available = sprint_available;
                enemy.m_sprint_recorder = m_sprint_recorder;
                enemy.m_sprint = m_sprint;
                enemy.m_Rect.moveTo(m_Rect_x, m_Rect_y);

                // Load HaveDone and target
                QJsonArray haveDoneArray = enemyObject["HaveDone"].toArray();
                for (const QJsonValue& value : haveDoneArray) {
                QJsonObject pointObject = value.toObject();
                PathPoint point(pointObject["x"].toInt(), pointObject["y"].toInt());
                enemy.HaveDone.append(point);
                }

                QJsonObject targetObject = enemyObject["target"].toObject();
                enemy.target.x = targetObject["x"].toInt();
                enemy.target.y = targetObject["y"].toInt();

                bs01++;
        }
    }

    // Load game state
    QJsonObject gameStateObject = json["game_state"].toObject();
    enemy_left = gameStateObject["enemy_left"].toInt();
    arrived = gameStateObject["arrived"].toInt();
    destroyed = gameStateObject["destroyed"].toInt();
    money = gameStateObject["money"].toInt();
    game_win = gameStateObject["game_win"].toBool();
    game_lose = gameStateObject["game_lose"].toBool();
    monster_appeared = gameStateObject["monster_appeared"].toInt();
    map_choose = gameStateObject["map_choose"].toInt();
    enemy_total = gameStateObject["enemy_total"].toInt();
    lose_condition = gameStateObject["lose_condition"].toInt();

}


int Mainscene::Pixel_toGrid(int x)
{
    return x/grid_width;
}

int Mainscene::Grid_toPixel(int x)
{
    return x*grid_width;
}

PathPoint Mainscene::Distance_Estimate(int Pix_x, int Pix_y)
{
    int x1,x2,y1,y2;
    x1 = Pix_x/grid_width;
    x2 = x1 + 1;
    y1 = Pix_y/grid_height;
    y2 = y1 + 1;

    double x = double(Pix_x)/grid_width;
    double y = double(Pix_y)/grid_height;

    double dis11 = std::sqrt(pow(x - double(x1),2) + std::pow(y - double(y1), 2));
    double dis12 = std::sqrt(pow(x - double(x1),2) + std::pow(y - double(y2), 2));
    double dis21 = std::sqrt(pow(x - double(x2),2) + std::pow(y - double(y1), 2));
    double dis22 = std::sqrt(pow(x - double(x2),2) + std::pow(y - double(y2), 2));

    double m = std::min({dis11,dis12,dis21,dis22});

    if(m == dis11){
        return PathPoint(x1,y1);
    }
    if(m == dis12){
        return PathPoint(x1,y2);
    }
    if(m == dis21){
        return PathPoint(x2,y1);
    }
    else{
        return PathPoint(x2,y2);
    }


}

bool Mainscene::isPathValidEvent(PathPoint current, QVector<PathPoint> &visited, int depth)
{
    if (depth > 100) {
        return false;
    }
    // test the end
    if (current.x == End[0].x && current.y == End[0].y) {
        return true;
    }

    visited.push_back(current);

    QVector<PathPoint> neighbors;
    for (const PathPoint& temp : EnemyPath) {
        if ((temp.x == current.x && std::abs(temp.y - current.y) == 1) ||
            (std::abs(temp.x - current.x) == 1 && temp.y == current.y)) {
                bool find = false;
                for (const PathPoint& v : visited) {
                if (v.x == temp.x && v.y == temp.y) {
                    find = true;
                    break;
                }

                }
                if (!find) {
                neighbors.push_back(temp);
                }
        }
    }

    for (const PathPoint& neighbor : neighbors) {
        if (isPathValidEvent(neighbor, visited, depth + 1)) {
                return true;
        }
    }

    visited.pop_back();
    return false;
}

void Mainscene::EnemyMoveEvent(Enemy& en)
{

    if(en.m_Free){
        return;
    }

    //update Pathpoint to coordinate with QRect
    if(std::abs(en.m_Rect.x() - Grid_toPixel(en.target.x)) < en.m_speed && std::abs(en.m_Rect.y() - Grid_toPixel(en.target.y)) < en.m_speed){
        en.m_Rect.moveTo(Grid_toPixel(en.target.x),Grid_toPixel(en.target.y));
        en.m_X = Pixel_toGrid(en.m_Rect.x());
        en.m_Y = Pixel_toGrid(en.m_Rect.y());
        en.HaveDone.push_back(en.target);

        // Detect The End first
        PathPoint endPoint = End[0];
        if(std::abs(en.m_X - endPoint.x) <= 1 && std::abs(en.m_Y - endPoint.y) <= 1){
                if(en.m_X == endPoint.x && en.m_Y == endPoint.y){
                en.Remove();
                arrived += 1;
                return;
                }

                // test the border
                int x = en.m_Rect.x();
                int y = en.m_Rect.y();

                int x_maximum = GAME_WIDTH ;//- en.m_Rect.width();
                int y_maximum = GAME_HEIGHT ;//- en.m_Rect.height();

                if(x <= 0 || x >= x_maximum || y <= 0 || y >= y_maximum){
                en.Remove();
                arrived += 1;
                return;
                }

        }


            // needs to update the target now


        //search the direction//
        QVector<PathPoint> PossiblePoint;
        //search the vicinity Pathpoint
        for(int i = 0; i < EnemyPath.size(); i++){
                PathPoint& temp = EnemyPath[i];
                //only the x or y is equal -> viciniy
                if((temp.x == en.m_X && std::abs(temp.y - en.m_Y ) == 1)||(std::abs(temp.x - en.m_X ) == 1 && temp.y == en.m_Y)){
                // Judge Whether the Point is used
                bool find = false;
                for(PathPoint value : en.HaveDone){
                    if(value.x == temp.x && value.y == temp.y){
                        find = true;
                        break;
                        // if true -> i++ -> next temp
                    }
                }
                if(!find)
                    PossiblePoint.push_back(temp);
                }
        }

        //Deal with the intersection

        //modify the PossiblePoint
        QVector<PathPoint> validPoints;
        for (const PathPoint& pt : PossiblePoint) {
                QVector<PathPoint> tempVisited = en.HaveDone;
                if (isPathValidEvent(pt, tempVisited)) {
                validPoints.push_back(pt);
                }
        }

        if (validPoints.isEmpty()) {
                return;
        }

        // creat random num
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, validPoints.size() - 1);
        int randomIndex = dis(gen);
        //qDebug() << randomIndex;
        //select the point
        en.target = validPoints[randomIndex];
        //
        //qDebug() << en.target.x << en.target.y;

        //



        }

    // move to the target
    int x_direction = 0;
    int y_direction = 0;

    if(en.target.x > en.m_X){
        x_direction = 1;
    }
    else if(en.target.x < en.m_X){
        x_direction = -1;
    }
    if(en.target.y > en.m_Y){
        y_direction = 1;
    }
    else if(en.target.y < en.m_Y){
        y_direction = -1;
    }
    // set the direction



    //update
    int new_x;
    int new_y;

    new_x = en.m_Rect.x() + x_direction * en.m_speed;
    new_y = en.m_Rect.y() + y_direction * en.m_speed;


    //final update
    en.m_Rect.moveTo(new_x,new_y);

    }

void Mainscene::EnemyToScene(Enemy* en)
{
    Enemy* obj;
    for(int i = 0; i < en->getChildStatic() ; i++){
        obj = &en[i];
        if(obj->m_Free){
            obj->m_Free = false;
            obj->HaveDone.clear();
            // Start Point Logic

            //start point may not be only one
            // creat random num
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, Start.size() - 1);
            int index = dis(gen);
            PathPoint startPoint = Start[index];//select the point

            obj->target = startPoint;
            obj->m_X = startPoint.x;
            obj->m_Y = startPoint.y;
            obj->m_Rect.moveTo(Grid_toPixel(obj->m_X),Grid_toPixel(obj->m_Y));

            break;
        }
    }



}

void Mainscene::TowerToScene(QVector<Tower*> tw, int x, int y)
{
    //cost
    if(tw[0]->cost > money){
        qDebug() << "no enough money";
        return;
    }
    /*
    for(Dtf01& dt : m_Dtf01s){
        if(dt.m_Free){
            return;
        }
        if(dt.)
    }*/

    Tower* obj;
    for(int i = 0; i < tw[0]->getChildStatic(); i++){
        obj = tw[i];
        if(obj->m_Free){
            obj->m_Free = false;
            money -= tw[0]->cost;//money
            obj->m_X = x;
            obj->m_Y = y;
            obj->m_Rect.moveTo(Grid_toPixel(obj->m_X),Grid_toPixel(obj->m_Y));
            break;
        }
    }
}

void Mainscene::WinOrLose_Detect()
{
    //qDebug() << enemy_total << ":" << destroyed + arrived;
    if(arrived >= lose_condition){
        game_lose = true;
        PlayingGamePlayer_1.stop();
        Player_lose.play();
        QTimer::singleShot(30, this, [this]() {
            stop = true;
        });
        //qDebug() << "you lose";
        //GameState = LOSESTATE;
    }
    if(enemy_total == destroyed + arrived){
        game_win = true;
        PlayingGamePlayer_1.stop();
        Player_win.play();

        //total money
        total_money += money;
        QTimer::singleShot(30, this, [this]() {
            stop = true;
        });

        //qDebug() << "you win";
        //GameState = WINSTATE;
    }
}

/*
void Mainscene::MagicToScene(Magic *mg, int Pix_x, int Pix_y)
{
    Magic* obj;
    for(int i = 0; i < mg->getChildStatic(); i++){
        obj = &mg[i];
        if(obj->m_Free){
            obj->m_Free = false;
            obj->m_Rect.moveTo(Pix_x,Pix_y);
            break;
        }
    }
}
*/
void Mainscene::PutEveryEnemies()
{
    if(enemy_left <= 0){
        return;
    }

    for(Enemy* KindOfEn : KindsOfEnemy){
        //interval control
        int interval = KindOfEn->getChildInterval();
        int& recorder =  Enemy_Interval_Map[KindOfEn];
        recorder ++ ;
        if(recorder < interval){
            return;
        }

        //appear the enemy
        recorder = 0;
        EnemyToScene(KindOfEn);
        enemy_left--;
    }
}

Enemy* Mainscene::SearchEnemyTarget(Tower &tw)
{
    if(tw.m_Free){
        qDebug() << "Tower is free";
        return nullptr;
    }

    int pos_x = tw.m_X;
    int pos_y = tw.m_Y;
    Enemy* min = nullptr;
    double m = 100000;
    //int index = -1;

    for(Enemy* KindOfEn : KindsOfEnemy){
        Enemy* obj = nullptr;

        for(int i = 0; i < KindOfEn->getChildStatic() ; i++){
            obj = &KindOfEn[i];
            if(obj->m_Free){
                continue;
            }

            double temp = distance(pos_x,pos_y,obj->m_X,obj->m_Y);
            if(temp < m){
                min = obj;
                m = temp;
                //index = i;
            }
        }
    }

    if(min != nullptr && m*50 <= tw.shootrange){
        //qDebug() << "min has found" << index;
        return min;
    }
    else
    {//qDebug() << "min is null";
        return nullptr;
    }
}

QVector<Enemy*> Mainscene::SearchAoeTarget(Enemy *en)
{
    QVector<Enemy*> temp;

    for(Bs01& bs : m_Bs01s){
        if(en == &bs or bs.m_Free){
            continue;
        }
        int pos_x = en->m_X;
        int pos_y = en->m_Y;
        if(bs.m_X == pos_x && bs.m_Y == pos_y){
            temp.push_back(&bs);
        }
    }
    return temp;

}



Tower *Mainscene::SearchTowerTarget(Enemy &en)
{
    if(en.m_Free){
        qDebug() << "Enemy is free";
        return nullptr;
    }

    int pos_x = en.m_X;
    int pos_y = en.m_Y;
    Tower* min = nullptr;
    double m = 100000;
    //int index = -1;


    /*
    for(Tower* KindOfTw : KindsOfTower){
        Tower* obj;
        for(int i = 0; i < KindOfTw->getChildStatic() ; i++){
            obj = &KindOfTw[i];

            if(obj->m_Free){
                continue;
            }
            double temp = distance(pos_x,pos_y,obj->m_X,obj->m_Y);
            if(temp < m){
                min = obj;
                m = temp;
                index = i;
            }
        }
    }*/
    ///
    /// 不知为何从KindsOfTower中读取的会未初始化，且只有mt01会发生这个情况
    /// 这里暂时用hardcode解决，之后再debug
    ///

    for(Mt01& obj : m_Mt01s){
            if(obj.m_Free){
                continue;
            }
            double temp = distance(pos_x,pos_y,obj.m_X,obj.m_Y);
            if(temp < m){
                min = &obj;
                m = temp;
        }
    }
    for(Dtf01& obj : m_Dtf01s){
        if(obj.m_Free){
                continue;
        }
        double temp = distance(pos_x,pos_y,obj.m_X,obj.m_Y);
        if(temp < m){
                min = &obj;
                m = temp;
        }
    }//hardcode


    if(min != nullptr && m*50 <= en.shootrange){
        //qDebug() << "min has found" << index;
        return min;
    }
    else
    {//qDebug() << "min is null";
        return nullptr;
    }
}


double Mainscene::distance(int x1, int y1, int x2, int y2)
{
    return std::sqrt(pow(x1 - double(x2),2) + std::pow(y1 - double(y2), 2));
}

void Mainscene::EnemyDeath_Event(Enemy *en)
{
    int px = en->m_Rect.x();
    int py = en->m_Rect.y();

    destroyed ++;
    money += en->destroy_gold_credit;
    Player_gold.play();

    if(en->Identity == BS01_IDENTITY){
        Bs01_death_Event(px,py);
    }
}

void Mainscene::Bs01_death_Event(int px, int py)
{
    for(Bs01_death& bs : m_Bs01_deaths){
        if(bs.m_Free){
                bs.m_Free = false;
                bs.pix_x = px;
                bs.pix_y = py;
                //px should be m_Rect.x() , py etc
        }
    }
}

void Mainscene::credit_apply()
{
    if(credit_freeze_enable){
        for(Mt01& mt : m_Mt01s){
            if(!mt.m_Free){
                mt.buff_freeze_collect = true;
            }
        }
    }
    if(credit_craze_enable){
        for(Dtf01& dt : m_Dtf01s){
            if(!dt.m_Free){
                dt.buff_craze_collect = true;
            }
        }
    }
}


void Mainscene::Initialize_UI()
{

    //Qpixmap
    UI_Archievement.load(ARCHIEVEMENT_PATH);
    UI_ArchievementDown.load(ARCHIEVEMENT_DOWN_PATH);
    UI_Credit.load(CREDIT_PATH);
    UI_CreditDown.load(CREDIT_DOWN_PATH);
    UI_Play.load(PLAY_PATH);
    UI_PlayDown.load(PLAY_DOWN_PATH);
    UI_Logo.load(LOGO_PATH);
    UI_MainMenu.load(MAINMENU_PATH);
    UI_WaveStation.load(WAVESTATION_PATH);
    UI_WaveStationLight.load(WAVESTATION_LIGHT_PATH);
    UI_Win.load(WIN_PATH);
    UI_Lose.load(LOSE_PATH);
    UI_Pause.load(PAUSE_PATH);
    UI_CHOOSE_MAP.load(CHOOSE_MAP_PATH);
    UI_Heart.load(HEART_PATH);
    UI_PowerBackground.load(POWER_BACKGROUND_PATH);
    UI_Bottom.load(BOTTOM_PATH);
    UI_Menu.load(MENU_PATH);
    UI_MenuDown.load(MENU_DOWN_PATH);
    UI_UpdateDown.load(UPDATE_DOWN_PATH);
    UI_Update.load(UPDATE_PATH);
    UI_Quit.load(QUIT_PATH);
    UI_QuitDown.load(QUIT_DOWN_PATH);
    UI_Gold.load(GOLD_PATH);
    UI_EnemyLeft.load(ENEMY_LEFT_PATH);
    UI_CreditBackground.load(CREDIT_BACKGROUND_PATH);
    UI_Credit_Craze_Grey.load(CREDIT_CRAZE_GRAY_PATH);
    UI_Credit_Craze_Normal.load(CREDIT_CRAZE_NORMAL_PATH);
    UI_Credit_Craze_Light.load(CREDIT_CRAZE_LIGHT_PATH);
    UI_Credit_Freeze_Light.load(CREDIT_FREEZE_LIGHT_PATH);
    UI_Credit_Freeze_Normal.load(CREDIT_FREEZE_NORMAL_PATH);
    UI_Credit_Freeze_Grey.load(CREDIT_FREEZE_GRAY_PATH);

    ///////QRect//////////
    UI_Archievement_Rect.setWidth(UI_Archievement.width());
    UI_Archievement_Rect.setHeight(UI_Archievement.height());
    UI_Archievement_Rect.moveTo(ARCHIEVEMENT_X,ARCHIEVEMENT_Y);
    //Wavestation
    UI_WaveStation_1_Rect.setWidth(UI_WaveStation.width());
    UI_WaveStation_1_Rect.setHeight(UI_WaveStation.height());
    UI_WaveStation_1_Rect.moveTo(STATION_1_X,STATION_1_Y);
    UI_WaveStation_2_Rect.setWidth(UI_WaveStation.width());
    UI_WaveStation_2_Rect.setHeight(UI_WaveStation.height());
    UI_WaveStation_2_Rect.moveTo(STATION_2_X,STATION_2_Y);
    UI_WaveStation_3_Rect.setWidth(UI_WaveStation.width());
    UI_WaveStation_3_Rect.setHeight(UI_WaveStation.height());
    UI_WaveStation_3_Rect.moveTo(STATION_3_X,STATION_3_Y);
    //Credit
    UI_Credit_Rect.setWidth(UI_Credit.width());
    UI_Credit_Rect.setHeight(UI_Credit.height());
    UI_Credit_Rect.moveTo(CREDIT_X,CREDIT_Y);
    //Play
    UI_Play_Rect.setWidth(UI_Play.width());
    UI_Play_Rect.setHeight(UI_Play.height());
    UI_Play_Rect.moveTo(PLAY_X,PLAY_Y);
    //Menu
    UI_Menu_Rect.setWidth(UI_Menu.width());
    UI_Menu_Rect.setHeight(UI_Menu.height());
    UI_Menu_Rect.moveTo(MENU_X,MENU_Y);
    //Update
    UI_Update_Rect.setWidth(UI_Update.width());
    UI_Update_Rect.setHeight(UI_Update.height());
    UI_Update_Rect.moveTo(UPDATE_X,UPDATE_Y);
    //Quit
    UI_Quit_Rect.setWidth(UI_Quit.width());
    UI_Quit_Rect.setHeight(UI_Quit.height());
    UI_Quit_Rect.moveTo(QUIT_X,QUIT_Y);
    //Credit
    UI_Credit_Freeze_Rect.setWidth(UI_Credit_Freeze_Grey.width());
    UI_Credit_Freeze_Rect.setHeight(UI_Credit_Freeze_Grey.height());
    UI_Credit_Freeze_Rect.moveTo(CREDIT_FREEZE_X,CREDIT_FREEZE_Y);
    UI_Credit_Craze_Rect.setWidth(UI_Credit_Craze_Grey.width());
    UI_Credit_Craze_Rect.setHeight(UI_Credit_Craze_Grey.height());
    UI_Credit_Craze_Rect.moveTo(CREDIT_CRAZE_X,CREDIT_CRAZE_Y);

    //Bool
    UI_Archievement_Click = false;
    UI_Credit_Click = false;
    UI_Quit_Click = false;
    UI_Update_Click = false;
    UI_Play_Click = false;
    UI_Menu_Click = false;
    UI_WaveStation_1_Click = false;
    UI_WaveStation_2_Click = false;
    UI_WaveStation_3_Click = false;
    UI_Credit_Freeze_Click = false;
    UI_Credit_Craze_Click = false;
}

void Mainscene::InitializeMedia()
{
    //Sound
    double vo = GAME_VOLUME;

    MainMenuPlayer.setSource(QUrl(SOUND_MAINMENU_PATH));
    MainMenuPlayer.setLoops(QMediaPlayer::Infinite);
    MainMenuPlayer.setAudioOutput(&AuOut1);
    AuOut1.setVolume(vo);

    PlayingGamePlayer_1.setSource(QUrl(SOUND_PLAYINGGAME_1_PATH));
    PlayingGamePlayer_1.setLoops(QMediaPlayer::Infinite);
    PlayingGamePlayer_1.setAudioOutput(&AuOut2);
    AuOut2.setVolume(vo);

    Player_gold.setSource(QUrl(SOUND_GOLD_PATH));
    Player_gold.setAudioOutput(&AuOut_gold);
    AuOut_gold.setVolume(vo);

    Player_win.setSource(QUrl(SOUND_WIN_PATH));
    Player_win.setAudioOutput(&AuOut_win);
    AuOut_win.setVolume(vo);

    Player_lose.setSource(QUrl(SOUND_LOST_PATH));
    Player_lose.setAudioOutput(&AuOut_lose);
    AuOut_lose.setVolume(vo);

}
