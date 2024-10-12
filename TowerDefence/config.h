#ifndef CONFIG_H
#define CONFIG_H

//GAME
#define GAME_WIDTH 960
#define GAME_HEIGHT 640
#define GAME_TITLE "TowerDefence 1.0 Version"
//Win or Lose
#define GAME_LOSE_ARRIVAL 3
//#define Resource_Path
#define GAME_RATE 20 // ms
#define GAME_ICON ":/Icon/Resource/Icon.png"
#define GAME_VOLUME 0.5 //sound
//Game State Identity
#define WINSTATE 10
#define LOSESTATE 9
#define PLAYINGSTATE 2
#define STARTSTATE 0
#define CHOOSEMAPSTATE 1
#define CREDITSTATE 3

//money
#define MONEY_INCREASE 1
#define MONEY_INTERVAL 100
#define GAME_START_MONEY 200
//credit cost
#define CREDIT_FREEZE_COST 200
#define CREDIT_CRAZE_COST 200

//hp bar
#define HEALTH_BAR_HEIGHT 5
//#define HEALTH_BAR_WIDTH 25

//consist lose health number
#define CONSIST_LOSEHEALTH_DAMAGE 5

////////////////////////////////////////////
// map
#define MAP_1_PATH ":/Background/Resource/map1_bd.png" //map1
#define MAP_2_PATH ":/Background/Resource/map0008y-hd.png"
#define MAP_3_PATH ":/Background/Resource/map0012y-hd.png"
//Json File
#define MAP1_JSON ":/JSON/Resource/map1.json"
#define MAP2_JSON ":/JSON/Resource/map2.json"
#define MAP3_JSON ":/JSON/Resource/map3.json"

//Magic
//mt01
#define MT01_MAGIC_PATH ":/Tower/mt01/Resource/mt01/thunderbu000%1.png"
//#define MT01_BULLET_SPEED 5
#define MT01_MAGIC_NUM 10
#define MT01_MAGIC_IMAGE_INTERVAL 20
#define MT01_MAGIC_IMAGE_MAX 9

//Tower
//mt01
#define MT01_PATH ":/Tower/mt01/Resource/mt01/mt01-%1.png"
#define MT01_NUM 10
#define MT01_HP 100
#define MT01_COST 50
#define MT01_DAMAGE 20
#define MT01_MAGIC_INTERVAL 100//50
#define MT01_SHOOTRANGE 250
#define MT01_SHOOTRANGE_COEFFICIENT 1.5
#define MT01_IMAGE_ATTACK_INTERVAL 10
#define MT01_IMAGE_ATTACK_MAX 5
#define MT01_IDENTITY 0
//#define MT01_MAX 5

//dtf01
#define DTF01_IMAGE_LEFT_PATH ":/Tower/dtf01/Resource/dtf01/dtf01atleft000%1.png"
#define DTF01_NUM 10
#define DTF01_HP 150
#define DTF01_COST 50
#define DTF01_DAMAGE 20
#define DTF01_SHOOTRANGE 50
#define DTF01_ATTACK_INTERVAL 50
#define DTF01_IMAGE_ATTACK_INTERVAL 20
#define DTF01_IMAGE_ATTACK_MAX 4
#define DTF01_IDENTITY 1


//Enemy
#define ENEMY_TOTAL 5
//bs01
#define BS01_PATH ":/Enemy/bs01/Resource/bs01/bs01mvc00%1.png"
#define BS01_ATTACK_PATH ":/Enemy/bs01/Resource/bs01/bs01at00%1.png"
#define BS01_DEAD_PATH ":/Enemy/bs01/Resource/bs01/bs01dd000%1.png"
#define BS01_NUM 10 // hardcode
#define BS01_HP 100
#define BS01_SPEED 1
#define BS01_SHOOTRANGE 50
#define BS01_ATTACK_INTERVAL 50
#define BS01_INTERVAL 200
#define BS01_DAMAGE 35
#define BS01_RAISEHAND_INTERVAL 30
#define BS01_DESTROY_GOLD_CREDIT 20
#define BS01_IMAGE_MOVE_INTERVAL 5
#define BS01_IMAGE_MOVE_MAX 16
#define BS01_IMAGE_ATTACK_MAX 12
#define BS01_IMAGE_ATTACK_INTERVAL 5
#define BS01_IMAGE_DEAD_INTERVAL 5
#define BS01_IMAGE_DEAD_MAX 9
#define BS01_STATE_MOVE 0
#define BS01_STATE_ATTACK 1
#define BS01_STATE_DEAD 2
#define BS01_DEATH_NUM 10
#define BS01_IDENTITY 1

//Buff
//const lose health
#define CONSIST_LOSEHEALTH_TOTAL 2
#define CONSIST_LOSEHEALTH_INTERVAL 10
//freeze
#define FREEZE_TOTAL_TIME 50
//fast
#define FAST_TOTAL_TIME 20
//Sprint
#define SPRINT_FAST_INTERVAL 100


//UI PATH
#define ARCHIEVEMENT_PATH ":/UI/Resource/UI/btnArchievement.png"
#define ARCHIEVEMENT_DOWN_PATH  ":/UI/Resource/UI/btnArchievementdown.png"
#define CREDIT_PATH ":/UI/Resource/UI/btnCredit.png"
#define CREDIT_DOWN_PATH ":/UI/Resource/UI/btnCreditdown.png"
#define PLAY_PATH   ":/UI/Resource/UI/btnPlay.png"
#define PLAY_DOWN_PATH ":/UI/Resource/UI/btnPlaydown.png"
#define LOGO_PATH ":/UI/Resource/UI/logo.png"
#define MAINMENU_PATH ":/UI/Resource/UI/mainMenuBg.png"
#define PAUSE_PATH ":/UI/Resource/UI/pauseBg-hd.png"
#define LOSE_PATH ":/UI/Resource/UI/lostBg-hd.png"
#define WIN_PATH ":/UI/Resource/UI/winBg-hd.png"
#define WAVESTATION_PATH ":/UI/Resource/UI/btnWaveStation-hd.png"
#define WAVESTATION_LIGHT_PATH ":/UI/Resource/UI/waveStationLight-hd.png"
#define CHOOSE_MAP_PATH ":/UI/Resource/UI/selectWaveBg-hd.png"
#define MENU_PATH ":/UI/Resource/UI/btnMenu-hd.png"
#define MENU_DOWN_PATH ":/UI/Resource/UI/btnMenudown-hd.png"
#define BOTTOM_PATH ":/UI/Resource/UI/bottom_bg-hd.png"
#define POWER_BACKGROUND_PATH ":/UI/Resource/UI/jiNengBg-hd.png"
#define HEART_PATH ":/UI/Resource/UI/xueTip-hd.png"
#define GOLD_PATH ":/UI/Resource/UI/gold-hd.png"
#define UPDATE_PATH ":/UI/Resource/UI/btnUpdateLv-hd.png"
#define UPDATE_DOWN_PATH ":/UI/Resource/UI/btnUpdateLvDown-hd.png"
#define QUIT_PATH ":/UI/Resource/UI/btnQuit-hd.png"
#define QUIT_DOWN_PATH ":/UI/Resource/UI/btnQuitdown-hd.png"
#define ENEMY_LEFT_PATH ":/UI/Resource/UI/enemyPoint-hd.png"
#define CREDIT_BACKGROUND_PATH ":/Credit/Resource/Credit/updataBg-hd.png"
#define CREDIT_FREEZE_GRAY_PATH ":/Credit/Resource/Credit/jN003Hui-hd.png"
#define CREDIT_FREEZE_NORMAL_PATH ":/Credit/Resource/Credit/jN003-hd.png"
#define CREDIT_FREEZE_LIGHT_PATH ":/Credit/Resource/Credit/jN003Liang-hd.png"
#define CREDIT_CRAZE_GRAY_PATH ":/Credit/Resource/Credit/jN001Hui-hd.png"
#define CREDIT_CRAZE_NORMAL_PATH ":/Credit/Resource/Credit/jN001-hd.png"
#define CREDIT_CRAZE_LIGHT_PATH ":/Credit/Resource/Credit/jN001Liang-hd.png"

//UI position
#define ARCHIEVEMENT_X 100
#define ARCHIEVEMENT_Y 200
#define CREDIT_X 130
#define CREDIT_Y 300
#define PLAY_X 100
#define PLAY_Y 400
#define QUIT_X 120
#define QUIT_Y 520
#define LOGO_X 100
#define LOGO_Y 0
#define STATION_1_X 50
#define STATION_1_Y 170
#define STATION_2_X 300
#define STATION_2_Y 320
#define STATION_3_X 650
#define STATION_3_Y 350
#define MENU_X 370
#define MENU_Y 400
#define UPDATE_X 410
#define UPDATE_Y 270
#define CREDIT_FREEZE_X 200
#define CREDIT_FREEZE_Y 200
#define CREDIT_CRAZE_X 600
#define CREDIT_CRAZE_Y 200
#define CREDIT_GOLD_X 735
#define CREDIT_GOLD_Y 25


//Sound
#define SOUND_MAINMENU_PATH "qrc:/Sound/Resource/Sound/rad.mp3"
#define SOUND_PLAYINGGAME_1_PATH "qrc:/Sound/Resource/Sound/161.mp3"
#define SOUND_GOLD_PATH "qrc:/Sound/Resource/Sound/jinBi.wav"
#define SOUND_KNIFE_ATTACK_PATH "qrc:/Sound/Resource/Sound/at.wav"
#define SOUND_MAGIC_ATTACK_PATH "qrc:/Sound/Resource/Sound/mt.wav"
#define SOUND_LOST_PATH "qrc:/Sound/Resource/Sound/lostBg.mp3"
#define SOUND_WIN_PATH  "qrc:/Sound/Resource/Sound/winBg.wav"
#define SOUND_LEVEL_UP_PATH  "qrc:/Sound/Resource/Sound/shengJi.wav"
#define SOUND_BUFF_PATH  "qrc:/Sound/Resource/Sound/lifePlus.wav"
#define SOUND_DEAD_PATH  "qrc:/Sound/Resource/Sound/friendlyDead.wav"

#endif // CONFIG_H

