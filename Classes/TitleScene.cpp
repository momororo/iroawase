#include "TitleScene.h"
#include <string.h>
#include "NendModule.h"
#include "NendInterstitialModule.h"
#include "AppCCloudPlugin.h"


#define selfFrame Director::getInstance() -> getWinSize()
#define KODOMO_FONT "KodomoRounded"

USING_NS_CC;
using namespace CocosDenshion;

//保存先の生成
UserDefault *userDef = UserDefault::getInstance();


/************* タイトルシーン ****************/

Label *titleLabel;
Sprite *titleRing;
Label *titleScore;

/*******************************************/



/***************** ゲーム中 ********************/
//衝突判定用オブジェクト
Sprite *circle;
Sprite *ring;

//ゲームボタン
Sprite *blueButton;
Sprite *redButton;
Sprite *greenButton;


Sprite *player;

//操作ボタン
Sprite *RGB;

//操作ボタンのダミー
Sprite *pRGB;

//境界線
Sprite *border;

//落下する輪
//Sprite *dropCircle;

//タッチポイントの保存
Point beganPoint;

Label *scoreLabel;
int score;
int bestScore;
int test;

/***********************************************/




/*************** ゲームオーバー ********************/

Label *gameOverLabel;
Sprite *retryBt;
Sprite *homeBt;
Label *result;
Label *resultLabel;
Label *omedeto;
Sprite *gameOverRing;

/************************************************/

int countTree;
int countDeff;

//AppCCloud用
Sprite *appCCloudBt;
bool appFlag;

//色判別用
auto pblue = Color4B(42, 127, 255,255);
auto pred = Color4B(255, 85, 85,255);
auto pgreen = Color4B(85, 255, 85,255);

auto blue = Color3B(42, 127, 255);
auto red = Color3B(255, 85, 85);
auto green = Color3B(85, 255, 85);
auto aqua = Color3B(42, 212, 255);
auto purple = Color3B(229, 128, 255);
auto yellow = Color3B(255, 221, 85);
auto white = Color3B(255, 255, 255);


//スタートボタンの出現フラグ
bool startFlag;
//ゲームスタートフラグ
bool gameStart;
//ゲームオーバーフラグ
bool gameOver;

//タッチムーブフラグ
bool touchMove;

bool newRecord;

//ゲームパターンのフラグ
bool startPattern;
bool startPattern1;
bool startPattern2;
bool startPattern3;

bool colorButton;
//RGBプレイヤーの動き
bool moving;
//RGBプレイヤーの伸縮運動
bool scaling;

//色のフラグ
bool blueFlag;
bool redFlag;
bool greenFlag;
bool aquaFlag;
bool yellowFlag;
bool purpleFlag;
bool whiteFlag;

bool contac;

Point getCircleVec2;

float gameSpeed;

int frameCount;

// MARK:タッチ範囲スプライトの透明度の設定
int clearColor = 0;
float playScale = 0.7;
float playerSpeed = 0.1;

Scene *TitleScene::createScene(){
    
    auto scene = Scene::createWithPhysics();
    auto layer = TitleScene::create();
    scene -> addChild(layer);
    
    
    auto world = scene -> getPhysicsWorld();
    cocos2d::Vect gravity;
    gravity.setPoint(0, -50);
    world -> setGravity(gravity);
    
    
    //物理体の可視化
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    
    return scene;
    
}

bool TitleScene::init(){
    
    
    if (!Layer::init()) {
        return false;
    }
    
    //背景色のグラデーション
    auto bgColor = LayerColor::create(Color4B(255,246,213,255));
    this -> addChild(bgColor);
    
    
    /************* タイトル各種設定 ********************/
    //タイトル表示設定
    setTitle();
    //スタート表示設定
    setStart();
    //ランキング表示設定
    setRanking();
    //とりあえず作った設定
    setTutorial();
    
    setAppCCloud();
    /*************************************************/
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("cursor1.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("decision7.mp3");
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.6f);

    
    //BGM
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bgmShelling.mp3",true);
    //音量
    //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
    
    
    startFlag =true;
    gameStart = false;
    gameOver = false;
    
    moving = false;
    
    //didsimulate
    this->scheduleUpdate();
    
    //MARK::ゲームセンターログイン
    GameCenterBridge::loginGameCenter();


    
    
    
    /**************　タッチイベント設定  ******************/
    
    //シングルタップ用リスナーを用意する
    auto listener = EventListenerTouchOneByOne::create();
    //listener -> setSwallowTouches(_swallowsTouches);
    
    
    //各イベントの割り当て
    listener -> onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan,this);
    listener -> onTouchMoved = CC_CALLBACK_2(TitleScene::onTouchMoved,this);
    listener -> onTouchEnded = CC_CALLBACK_2(TitleScene::onTouchEnded,this);
    
    //イベントディスパッチャようにリスナーを追加する
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
    
    /*************　　タッチイベント設定  終 ****************/
    
    /*************　  衝突イベント設定   ****************/
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener -> onContactBegin = CC_CALLBACK_1(TitleScene::onContactBegin,this);
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(contactListener,this);
    
    /*************　  衝突イベント設定  終 ****************/
    
    
    //MARK::nendの設定
    //ネンドの呼び出し(ヘッダー)
    char apiKey[] = "5aa579f521da85a055c5076641efc68202d5e8e2";
    char spotID[] = "262876";
    NendModule::createNADViewBottom(apiKey, spotID);
    
    //ネンドの呼び出し(飛び出す)
    char interstitialApiKey[] = "6b027f392e0cf11d378908fc4027f1755d6422ad";
    char interstitialSpotID[] = "266067";
    NendInterstitialModule::createNADInterstitial(interstitialApiKey, interstitialSpotID);
    
    
    
    
    return true;
    
    
}

//MARK::AppCCloudの設定と削除
void TitleScene::setAppCCloud(){
    
    appCCloudBt = Sprite::create("other.png");
    appCCloudBt -> setPosition(Vec2((selfFrame.width)-(appCCloudBt->getContentSize().width*2/3),(selfFrame.height)-(appCCloudBt->getContentSize().height*2/3)));
    this -> addChild(appCCloudBt);
    
    appFlag = true;

}

void TitleScene::removeAppCCloud(){
    
    appCCloudBt -> removeFromParent();
    
    appFlag = false;

}



#pragma mark-
#pragma mark touchBegan

bool TitleScene::onTouchBegan(Touch *touch, Event *unused_event){
    
    //タップ開始時の処理
    CCLOG("touchBegan");
    
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);

    //ゲームスタート時のタッチポイント取得(タッチムーブで下にスワイプしたか検知するため)
    if (gameStart == true) {
        
        beganPoint = touchPoint;
        
    }
    
    return true;
    
}

#pragma mark-
#pragma mark touchMoved

void TitleScene::onTouchMoved(Touch *touch, Event *unused_event){
    
    //スワイプ中の処理
    CCLOG("touchMoved");
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);
    
    if (gameStart == true) {
    
        if (RGB -> getBoundingBox().containsPoint(touchPoint)) {
            if ((beganPoint.y-touchPoint.y > 0)){
                
                RGB -> setPosition(Vec2(pRGB->getPosition().x,pRGB->getPosition().y-50));
                touchMove = true;
                
            }else{
                
                return;
                
            }
            
            return;
            
        }
    }
}


#pragma mark-
#pragma mark touchEnded

void TitleScene::onTouchEnded(Touch *touch, Event *unused_event){
    
    //タップ終了時
    CCLOG("touchEnded");
    
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);
    
    
    //スタートボタンがタップされたとき
    //スタートフラグで条件指定(処理条件がうまくハマらず、ノードが消えてもBoundingBoxが有効になったため)
    if (startFlag == true && start -> getBoundingBox().containsPoint(touchPoint)) {
        
        startFlag = false;
        
        setGamestart();
        removeAppCCloud();
        
        SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");

        
    }
    
    //チュートリアルボタンをタップ
    if (startFlag == true && tutorial -> getBoundingBox().containsPoint(touchPoint)) {
        
        //アニメーション付き
        float duration = 2.0f;  //開始→終了にかける時間
        Scene* nextScene = CCTransitionFade::create(duration, TutorialScene::createScene());
        
        Director::getInstance()->replaceScene(nextScene);
        SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");

        
    }
    
    //MARK::ゲームセンターにアクセス
    if (startFlag == true && ranking -> getBoundingBox().containsPoint(touchPoint)) {
        
        GameCenterBridge::openRanking();
        SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");

        
    }
    
    //MARK::appCCloudBtを押された時の動作
    
    if (appFlag == true && appCCloudBt-> getBoundingBox().containsPoint(touchPoint)) {
    
        AppCCloudPlugin::Ad::openAdListView();
    
    }
    
    
    
    //ホーム画面に戻るボタンをおした時
    if (gameOver == true && homeBt->getBoundingBox().containsPoint(touchPoint)) {
        
        gameOver = false;
        newRecord = false;
        //アニメーション付き
        float duration = 2.0f;  //開始→終了にかける時間
        Scene* nextScene = CCTransitionFade::create(duration, TitleScene::createScene());
        
        Director::getInstance()->replaceScene(nextScene);
        SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");

    }
    
    
    
    //リトライボタンを押した時
    if (gameOver == true && retryBt->getBoundingBox().containsPoint(touchPoint)) {
        
        gameOver = false;
        
        removeGameOver();
        removeAppCCloud();
        SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");

        
    }
    

    
    
    
    
    /******************* ゲームボタン設定 ************************/
    
    //青色を押したときの動作
    //ボタンがないとき
    if (gameStart == true) {
        
        Point touchRGB = RGB -> convertToNodeSpace(touchPoint);
        
        
        if (touchMove == false) {
        
            //MARK:青ボタンをタップしたとき
            if(blueButton -> getBoundingBox().containsPoint(touchRGB)){
            
                if(border && moving == false){
                
                    //背景が赤のとき
                    if (redFlag == true) {
                    
                        //紫に変更
                        redFlag = false;
                        purpleFlag = true;
                        colorButton = true;
                    
                        //背景が緑のとき
                    }else if (greenFlag == true){
                    
                        //水色に変更
                        greenFlag = false;
                        aquaFlag = true;
                        colorButton = true;
                    
                    //背景が黄色のとき
                    }else if (yellowFlag == true){
                    
                        //白に変更
                        yellowFlag = false;
                        whiteFlag = true;
                        colorButton = true;
                    
                        //背景が紫のとき
                    }else if (purpleFlag == true || aquaFlag == true || whiteFlag == true){
                    
                        blueFlag = true;
                        purpleFlag = false;
                        aquaFlag = false;
                        whiteFlag = false;
                        colorButton = true;
                    
                    }else if(blueFlag == true){
                    
                        return;
                    
                    }
                
                    return;
                }
            
                return;
            }
        
            //MARK:赤のボタン設定
            if (redButton-> getBoundingBox().containsPoint(touchRGB)){
            
                if(border && moving == false){
                
                    //青色のとき
                    if (blueFlag == true) {
                    
                        blueFlag = false;
                        purpleFlag = true;
                        colorButton = true;
                    
                    }else if (greenFlag == true){
                    
                        greenFlag = false;
                        yellowFlag = true;
                        colorButton = true;
                    
                    }else if (aquaFlag == true){
                    
                        aquaFlag = false;
                        whiteFlag = true;
                        colorButton = true;
                    
                    }else if (purpleFlag == true || yellowFlag ==true || whiteFlag == true){
                    
                        purpleFlag = false;
                        yellowFlag = false;
                        whiteFlag = false;
                        redFlag = true;
                        colorButton = true;
                    
                    }else if(redFlag == true){
                        
                        return;
                    
                    }
                
                    return;
                
                }
            
                return;
            
            }
        
            //MARK:緑ボタン
            if (greenButton-> getBoundingBox().containsPoint(touchRGB)){
            
                if(border && moving == false){
                
                    if (blueFlag == true) {
                    
                        blueFlag = false;
                        aquaFlag = true;
                        colorButton = true;
                    
                    }else if (redFlag == true){
                    
                        redFlag = false;
                        yellowFlag = true;
                        colorButton = true;
                    
                    }else if (purpleFlag == true){
                    
                        purpleFlag = false;
                        whiteFlag = true;
                        colorButton = true;
                    
                    }else if (aquaFlag == true || yellowFlag == true || whiteFlag == true){
                    
                        aquaFlag = false;
                        yellowFlag = false;
                        whiteFlag = false;
                        greenFlag = true;
                        colorButton = true;
                        
                    }else if(greenFlag == true){
                    
                        return;
                    
                    }
                
                    return;
                
                }
            
                return;
            
            }
        
            return;
        
        }else{
        
            //MARK:攻撃開始
            touchMove = false;
            RGB -> setPosition(Vec2(pRGB->getPosition().x,pRGB->getPosition().y));
            setShooting();

        }
        
        return;
        
    }
    
    /******************************** ボタン設定 終 **********************************/

    
    
    
    
    
    
}





#pragma mark-
#pragma mark 衝突判定
bool TitleScene::onContactBegin(cocos2d::PhysicsContact& contact){
    
    
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    CCLOG("衝突判定スタート");
    
    
    /*
     tag1 -> 射撃ボール
     tag2 -> RGBの円
     tag3 -> 落下してくる輪
     */

    /*********************************** 本体の衝突 ************************************************/
    //MARK:輪が本体に衝突したか判定する
    if((nodeA->getTag() == 2 || nodeB->getTag() == 2)&&(nodeA->getTag() == 3 || nodeB->getTag() == 3)){
        
        if (test != (int)dropVector.size()) {
            return true;
        }
        
        //RGBの円
        Sprite *ring;
        //落下してくる輪
        Sprite *circle;
        
        if (nodeA -> getTag() == 2) {
        
            ring = (Sprite*)nodeA;
            circle = (Sprite*)nodeB;
            
        }else{
        
            ring = (Sprite*)nodeB;
            circle = (Sprite*)nodeA;
        
        }
        
        //リングと輪の色が同じ場合
        if (nodeA->getName() == nodeB->getName()) {
            
            //ノードAがリングだったら
            if (nodeA->getTag() == 2) {
                
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                    
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto remove = RemoveSelf::create(true);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                    
                auto sequence = Sequence::create(spawn,remove, NULL);
                    
                circle-> runAction(sequence);
                
                //int i = (int)dropVector.size();
                /*
                if (i == 1) {
                    
                    dropVector.erase(i);
                    
                }else if(i > 1){
                    
                    dropVector.erase(i - (i-1));
                    
                }*/
                
                dropVector.erase(0);
                SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");
                
                //MARK::スコア加算処理
                score += 1;
                std::string scoreStr = StringUtils::format("%d",score);
                scoreLabel -> setString(scoreStr);
                CCLOG("１点追加で現在:%d",score);
            
                
            }else if (nodeB->getTag() == 2){
                
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto remove = RemoveSelf::create(true);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                
                auto sequence = Sequence::create(spawn,remove, NULL);
                
                circle-> runAction(sequence);
                /*
                int i = (int)dropVector.size();
                
                if (i == 1) {
                    
                    dropVector.erase(i);
                    
                }else if(i > 1){
                    
                    dropVector.erase(i - (i-1));
                    
                }*/
                
                SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");
                
                dropVector.erase(0);
                
                //MARK::スコア加算処理
                score += 1;
                std::string scoreStr = StringUtils::format("%d",score);
                scoreLabel -> setString(scoreStr);
                CCLOG("１点追加で現在:%d",score);
 
            
            }
            
            return true;
            
        }else{
            
            //ノードAがリングだったら
            if (nodeA->getTag() == 2) {
                
                /*************** 落ちてくる輪の削除 **************************/
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto remove = RemoveSelf::create(true);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                
                auto sequence = Sequence::create(spawn,remove, NULL);
                
                circle-> runAction(sequence);
                
                /*************** 落ちてくる輪の削除 終**************************/
                
                CCLOG("ゲームオーバー処理要素数:%d",(int)dropVector.size());
                
                //ゲームオーバー処理
                gameStart = false;

                ring -> setColor(Color3B::BLACK);

                
                setGameover();
                
            }else if (nodeB->getTag() == 2){
                
                /*************** 落ちてくる輪の削除 **************************/
                
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto remove = RemoveSelf::create(true);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                
                auto sequence = Sequence::create(spawn,remove, NULL);
                
                circle-> runAction(sequence);
                
                /*************** 落ちてくる輪の削除 **************************/
                
                CCLOG("ゲームオーバー処理要素数:%d",(int)dropVector.size());
                
                //ゲームオーバー処理
                gameStart = false;

                ring -> setColor(Color3B::BLACK);
                
                setGameover();
            
            }
        
            return true;
            
        }
        
        return true;
    
    }
    
    /*********************************** 本体の衝突 (終) ************************************************/
    
    
    
    /********************************* 球とリングの衝突 *****************************************/
    //MARK:球とリングの衝突を判定
    if ((nodeA-> getTag() == 1 || nodeB -> getTag() == 1) && (nodeA->getTag() == 3 || nodeB->getTag()==3)) {
        
        Sprite *circle;
        Sprite *ball;
        
        //ノードにスプライトを当てはめる
        if (nodeA -> getTag() == 1) {
            ball = (Sprite*)nodeA;
            circle = (Sprite*)nodeB;
        }else{
            ball = (Sprite*)nodeB;
            circle = (Sprite*)nodeA;
        }
        
        //球とリングの色が同じ時の処理
        if (nodeA->getName() == nodeB->getName()) {
            
            //リングを効果をつけて削除(nodeAがボールの場合)
            if (nodeA->getTag()== 1) {
                
                //球とリングの同時消去
                auto remove = RemoveSelf::create(true);
                ball -> runAction(remove);
                
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                
                auto sequence = Sequence::create(spawn,remove->clone(), NULL);
                
                circle-> runAction(sequence);
                
                /*
                int i = (int)dropVector.size();
                
                if (i == 1) {
                    
                    dropVector.erase(i);
                    
                }else if(i > 1){
                    
                    dropVector.erase(i - (i-1));
                    
                }*/
                
                SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");
                
                dropVector.erase(0);
                
                //MARK::スコア加算処理
                score += 10;
                std::string scoreStr = StringUtils::format("%d",score);
                scoreLabel -> setString(scoreStr);
                CCLOG("１0点追加で現在:%d",score);
                
            //リングを効果をつけて削除(nodeBがボールの場合)
            }else if(nodeB->getTag()==1){
                
                //削除アクション
                auto remove = RemoveSelf::create(true);
                
                //ボールの削除
                ball-> runAction(remove);
                
                //リングの動きを止める
                circle -> stopAllActions();
                
                //リングの物理体削除
                circle->getPhysicsBody()->removeFromWorld();

                //拡大アクション
                auto scale = ScaleTo::create(1, 1.5);
                //フェードアウトアクション
                auto fadeOut = FadeOut::create(1);
                //拡大・フェードアウトの同時アクション
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                //同時アクション後、リングの削除
                auto sequence = Sequence::create(spawn,remove->clone(), NULL);
                
                //リングにアクションの適用
                circle-> runAction(sequence);
                /*
                int i = (int)dropVector.size();
                
                if (i == 1) {
                    
                    dropVector.erase(i);
                    
                }else if(i > 1){
                
                    dropVector.erase(i - (i-1));
                
                }*/
                
                SimpleAudioEngine::getInstance()->playEffect("decision7.mp3");
                
                dropVector.erase(0);
                
                //MARK::スコア加算処理
                score += 10;
                std::string scoreStr = StringUtils::format("%d",score);
                scoreLabel -> setString(scoreStr);
                CCLOG("１0点追加で現在:%d",score);

                
            }
            
        }else{
            
            CCLOG("ちがういろでーす");
            if (nodeA->getTag()== 1) {
                
                auto remove = RemoveSelf::create(true);
                ball -> runAction(remove);
    
                
            }else if(nodeB->getTag()==1){
            
                auto remove = RemoveSelf::create(true);
                ball -> runAction(remove);
            
            }
            
            return true;
        
        }
        
        return true;
        
    }
    /********************************* 球とリングの衝突 終*****************************************/

    return true;

}

#pragma mark-
#pragma mark タイトル各種設定

/***********************************************************
 各種オープニングの動作設定
 **********************************************************/


//タイトル
void TitleScene::setTitle(){
    
    titleLabel = Label::createWithSystemFont("COLORS", "Arial", 130);
    titleLabel -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*4/5));
    this -> addChild(titleLabel,100);
    
    
    titleRing = Sprite::create("titleRing.png");
    titleRing -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*2/5));
    titleRing -> setColor(blue);
    titleRing -> setScale(1.7);
    
    this -> addChild(titleRing,0);
    
    auto tintpurple = TintTo::create(1,229, 128, 255);
    auto tintRed = TintTo::create(1,255, 85, 85);
    auto tintYellow = TintTo::create(1,255, 221, 85);
    auto tintGreen = TintTo::create(1,85, 255, 85);
    auto tintAqoa = TintTo::create(1,42, 212, 255);
    auto tintBlue = TintTo::create(1,42, 127, 255);
    
    auto sequence = Sequence::create(tintpurple,tintRed,tintYellow,tintGreen,tintAqoa,tintBlue, NULL);
    
    auto repeat = RepeatForever::create(sequence);
    
    titleRing ->runAction(repeat);

    /*
    青 = Color3B(42, 127, 255);
    auto red = Color3B(255, 85, 85);
    auto green = Color3B(85, 255, 85);
    auto aqua = Color3B(42, 212, 255);
    auto purple = Color3B(229, 128, 255);
    auto yellow = Color3B(255, 221, 85);
    auto white = Color3B(255, 255, 255);
    */
    
    
    bestScore = userDef -> getIntegerForKey("bestScore");
    
    std::string scoreStr = StringUtils::format("BEST: %d",bestScore);
    
    titleScore = Label::createWithSystemFont(scoreStr,KODOMO_FONT, 70);
    titleScore -> setPosition(Vec2(selfFrame.width/2,selfFrame.height/5));
    titleScore -> setColor(Color3B::GRAY);
    this->addChild(titleScore);

}



//オープニングのスタート動作
void TitleScene::setStart(){
    
    start = Sprite::create("startBt.png");
    start -> setPosition(Vec2(selfFrame.width/2, selfFrame.height*0.55));
    this -> addChild(start);
    
    
}



//オープニングのランキング動作
void TitleScene::setRanking(){
    
    ranking = Sprite::create("rankingBt.png");
    ranking -> setPosition(Vec2(selfFrame.width*2/7, selfFrame.height*0.35));
    this -> addChild(ranking);

}


//オープニングのチャレンジ動作
void TitleScene::setTutorial(){
    
    tutorial = Sprite::create("tutorialBt.png");
    tutorial -> setPosition(Vec2(selfFrame.width*5/7, selfFrame.height*0.35));
    this -> addChild(tutorial);
       
}


//ゲーム準備の動作
void TitleScene::setGamestart(){
    
    CCLOG("おされたよー");
    
    auto fadeOut = FadeOut::create(1);
    auto remove = RemoveSelf::create(true);
    
    auto seqTitle = Sequence::create(fadeOut,remove, NULL);
    
    
    auto move = MoveTo ::create(1, Vec2(selfFrame.width/2, selfFrame.height*0.26));
    
    auto play = CallFunc::create([&](){
        
        //フラグをOFF
        startFlag = false;
        
        //ゲームスタートフラグON
        gameStart = true;
        
        startPattern = true;
        
        
        
        setRGB();
        setPlayer();
        
    });
    
    auto seqGameStart = Sequence::create(move,fadeOut,play,remove->clone(), NULL);
    auto sequence = Sequence::create(move->clone(),fadeOut->clone(),remove->clone(), NULL);

    
    titleLabel ->runAction(seqTitle);
    titleRing -> runAction(seqTitle->clone());
    titleScore ->runAction(seqTitle->clone());
    
    start->runAction(seqGameStart);
    ranking -> runAction(sequence);
    tutorial -> runAction(sequence->clone());
   
}







/***********************************************************
 各種オープニングの動作設定　終
 **********************************************************/



#pragma mark-
#pragma mark ゲームシーン設定

// MARK:操作ボタンの設定
void TitleScene::setRGB(){
    
    RGB = Sprite::create("RGB.png");
    RGB -> setPosition(Vec2(selfFrame.width/2, selfFrame.height*0.26));
    RGB -> setScale(playScale);
    this -> addChild(RGB,100);
    
    pRGB = Sprite::create();
    pRGB -> setTextureRect(Rect(0, 0, 10, 10));
    pRGB -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*0.26));
    pRGB -> setColor(Color3B::BLACK);
    pRGB -> setOpacity(clearColor);
    this -> addChild(pRGB);
    
    //青ボタン
    blueButton = Sprite::create();
    blueButton -> setTextureRect(Rect(0,0,310,240));
    blueButton -> setPosition(Vec2(RGB->getContentSize().width/2, RGB->getContentSize().height*3/4+50));
    blueButton -> setColor(Color3B::BLUE);
    blueButton -> setOpacity(clearColor);
    RGB -> addChild(blueButton,200);
    
    //赤ボタン
    redButton = Sprite::create();
    redButton -> setTextureRect(Rect(0,0,310,240));
    redButton -> setPosition(Vec2(RGB->getContentSize().width*5/6, RGB->getContentSize().height/5));
    redButton -> setColor(Color3B::RED);
    redButton -> setRotation(-55);
    redButton -> setOpacity(clearColor);
    RGB -> addChild(redButton,200);
    
    //緑ボタン
    greenButton = Sprite::create();
    greenButton -> setTextureRect(Rect(0,0,310,240));
    greenButton -> setPosition(Vec2(RGB->getContentSize().width/6, RGB->getContentSize().height/5));
    greenButton -> setColor(Color3B::GREEN);
    greenButton -> setRotation(55);
    greenButton -> setOpacity(clearColor);
    RGB -> addChild(greenButton,200);
    
    //境界線
    border = Sprite::create("ring.png");
    border -> setPosition(Vec2(RGB->getContentSize().width/2,RGB->getContentSize().height/2-20));
    border -> setColor(Color3B::WHITE);
    //RGB-> addChild(border);
    
    
    
    
    border -> setName("white");
    border -> setTag(2);
    
    //物理体の生成
    auto borderBody = PhysicsBody::createCircle(border->getContentSize().width/2);
    borderBody->setDynamic(false); // 重力の影響を受けない
    borderBody->setEnable(true);
    
    borderBody->setCategoryBitmask(0x01);
    borderBody->setCollisionBitmask(0);
    borderBody->setContactTestBitmask(0x02);
    
    border->setPhysicsBody(borderBody);
    
    RGB ->addChild(border);
    
    
    
    //スコア
    score = 0;
    std::string scoreStr = StringUtils::format("%d",score);
    
    //???:スコアの位置がいまいちわからん
    scoreLabel = Label::createWithSystemFont(scoreStr.c_str(),KODOMO_FONT, 100);
    scoreLabel -> setTextColor(Color4B::GRAY);
    scoreLabel -> setAnchorPoint(Vec2(1,0.5));
    scoreLabel -> setPosition(Vec2(selfFrame.width-scoreLabel->getContentSize().width,selfFrame.height-scoreLabel->getContentSize().height/2));
    
    this -> addChild(scoreLabel,1000);

    CCLOG("ゲームはじめの要素数:%d",(int)dropVector.size());

    
    
    
    moving = false;
    whiteFlag = true;
    
    
    
}

// MARK:プレイヤーの作成
void TitleScene::setPlayer(){
    
    player = Sprite::create("player.png");
    player -> setPosition(Vec2(RGB->getContentSize().width/2,RGB->getContentSize().height/2-25));
    player -> setScale(playScale);
    RGB -> addChild(player,1000);

}

//MARK:射撃
void TitleScene::setShooting(){
    
    auto ball = Sprite::create("whiteBall.png");
    ball -> setPosition(selfFrame.width/2,pRGB->getPosition().y + RGB->getContentSize().height/2);
    ball -> setColor(border->getColor());
    ball -> setScale(0.5);
    
    ball ->setTag(1);
    ball -> setName(border->getName());
    
    auto ballBody = PhysicsBody::createCircle((ball->getContentSize().width*ball->getScale())/2);
    ballBody->setDynamic(false); // 重力の影響を受けない
    ballBody->setEnable(true);
    
    ballBody->setCategoryBitmask(0x01);
    ballBody->setCollisionBitmask(0);
    ballBody->setContactTestBitmask(0x02);
    
    ball->setPhysicsBody(ballBody);
    
    this -> addChild(ball,1000);
    
    auto moveBall = MoveTo::create(1, Vec2(selfFrame.width/2,selfFrame.height+ball->getContentSize().height/2));
    
    auto remove = RemoveSelf::create();
    
    auto sequence = Sequence::create(moveBall,remove, NULL);
    
    ball -> runAction(sequence);
    
    



}

#pragma mark-
#pragma mark:ゲームオーバー設定
void TitleScene::setGameover(){
    
    //落下物の動作停止
    this->unschedule(schedule_selector(TitleScene::setPattern1));
    
    border ->getPhysicsBody() -> removeFromWorld();
    
    //その他の落下物すべて削除
    for(int i = 1; i< (int)dropVector.size();i++){
        
        CCLOG("ゲームオーバー設定　%d回目　要素数:%d",i,(int)dropVector.size());

        
        dropVector.at(i) -> runAction(RemoveSelf::create(true));
    
    }
    
        //配列の先頭のオブジェクトの削除
        dropVector.clear();
        
        CCLOG("残りの要素数:%d",(int)dropVector.size());

    
    
    gameStart = false;
    
    //操作ボタンの機能停止
    /**********************************************************/
    auto blink = Blink::create(3, 3);
    
    auto fadeOut = FadeOut::create(1);
    
    auto remove = RemoveSelf::create(true);
    
    auto fadeOutRGB = CallFunc::create([&](){
        
        auto fadeOut = FadeOut::create(1);
        auto remove = RemoveSelf::create(true);
        
        auto seqRGB = Sequence::create(fadeOut,remove, NULL);
        
        RGB->runAction(seqRGB);
        pRGB -> removeFromParent();
        
    });
    
    auto mgk = CallFunc::create([&](){
        
        makeGameOver();
        
    });
    
    auto spawn = Spawn::create(fadeOut,fadeOutRGB, NULL);
    
    auto seqRing = Sequence::create(blink,spawn,remove,mgk, NULL);
    
    auto scoreSeq = Sequence::create(fadeOut->clone(),remove->clone(), NULL);
    
    scoreLabel -> runAction(scoreSeq);
    border-> runAction(seqRing);
    /***********************************************************/
    
    }


void TitleScene::makeGameOver(){
    
    /******** ラベル＆リトライ＆ホームボタンの設定 *******/
    
    gameOverLabel = Label::createWithSystemFont("GAME OVER","Arial", 100);
    gameOverLabel -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*4/5));
    this -> addChild(gameOverLabel,10);
    
    
    /*********************************************/
    
    gameOverRing = Sprite::create("titleRing.png");
    gameOverRing -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*2/5));
    gameOverRing -> setColor(blue);
    gameOverRing -> setScale(1.7);
    
    this -> addChild(gameOverRing,0);
    
    auto tintpurple = TintTo::create(1,229, 128, 255);
    auto tintRed = TintTo::create(1,255, 85, 85);
    auto tintYellow = TintTo::create(1,255, 221, 85);
    auto tintGreen = TintTo::create(1,85, 255, 85);
    auto tintAqoa = TintTo::create(1,42, 212, 255);
    auto tintBlue = TintTo::create(1,42, 127, 255);
    
    auto sequence = Sequence::create(tintpurple,tintRed,tintYellow,tintGreen,tintAqoa,tintBlue, NULL);
    
    auto repeat = RepeatForever::create(sequence);
    
    gameOverRing ->runAction(repeat);
    
    /*******************************************************************/
    
    retryBt = Sprite::create("retryBt.png");
    retryBt -> setPosition(Vec2(selfFrame.width*3/4,selfFrame.height/3));
    this->addChild(retryBt,10);
    
    homeBt = Sprite::create("homeBt.png");
    homeBt ->  setPosition(Vec2(selfFrame.width*1/4,selfFrame.height/3));
    this -> addChild(homeBt,10);

    
    
    /******** ラベル＆リトライ＆ホームボタンの設定 終 *******/
    
    
    //MARK::スコア登録
    if(bestScore < score){
        
        bestScore = score;
        
        //登録
        userDef->setIntegerForKey("bestScore", bestScore);
        omedeto = Label::createWithSystemFont("Best Score!!", KODOMO_FONT, 60);
        omedeto -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*2/3));
        omedeto ->setColor(red);
        this->addChild(omedeto);
        
        auto blink = Blink::create(1, 1);
        
        auto repeat = RepeatForever::create(blink);
        
        omedeto -> runAction(repeat);
        
        newRecord = true;
        
        GameCenterBridge::postHighScore("RGB.BestScore", bestScore);
    }
    
    std::string scoreStr = StringUtils::format("%d",score);
    result = Label::createWithSystemFont(scoreStr.c_str(), KODOMO_FONT, 100);
    result ->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2));
    result -> setColor(Color3B::GRAY);
    this -> addChild(result);
    
    
    resultLabel = Label::createWithSystemFont("SCORE", KODOMO_FONT, 100);
    resultLabel ->setPosition(Vec2(selfFrame.width/2,selfFrame.height/2+result->getContentSize().height));
    resultLabel -> setColor(Color3B::GRAY);
    this -> addChild(resultLabel);
    
    gameOver = true;
    
    //MARK::nend飛だし広告の表示
    NendInterstitialModule::showNADInterstitialView();
    setAppCCloud();
    
}

void TitleScene::removeGameOver(){

    auto remove = RemoveSelf::create(true);
    auto fadeOut = FadeOut::create(1);
    
    
    auto retryScene = CallFunc::create([&](){
        
        
        setRGB();
        setPlayer();
        
        gameStart = true;
        startPattern = true;
        
    });
    
    
    auto sequence = Sequence::create(fadeOut,remove, NULL);
    auto retrySequence = Sequence::create(fadeOut->clone(),remove->clone(),retryScene, NULL);
    

    
    
    
    gameOverLabel ->runAction(sequence);
    gameOverRing -> runAction(sequence->clone());
    resultLabel -> runAction(sequence->clone());
    result -> runAction(sequence->clone());
    homeBt -> runAction(sequence->clone());
    retryBt -> runAction(retrySequence);
    
    if (newRecord == true) {
        
        newRecord = false;
        omedeto ->runAction(sequence->clone());
    }
    

    

}


#pragma mark-
#pragma mark:フレーム管理

void TitleScene::update( float frame )
{
    
    //MARK:プレイヤーの移動
    
    if (colorButton == true) {
        
        //青
        if (blueFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width/2, RGB->getContentSize().height*5/6));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(blue);
                border -> setName("blue");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
            
                moving = false;
                scaling = true;
            
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
            
        }
        
        //赤
        if (redFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*4/5, RGB->getContentSize().height*1/5));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(red);
                border -> setName("red");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
            
        }
        
        //緑
        if (greenFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;

            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*1/5, RGB->getContentSize().height*1/5));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(green);
                border -> setName("green");
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
        }
        
        
        
        //水色
        if (aquaFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*0.32, RGB->getContentSize().height*0.55));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(aqua);
                border -> setName("aqua");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
        }
        
        //紫色
        if (purpleFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*0.67, RGB->getContentSize().height*0.55));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(purple);
                border -> setName("purple");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
        }
        
        //黄色
        if (yellowFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*0.495, RGB->getContentSize().height*0.25));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(yellow);
                border -> setName("yellow");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
        }
        
        //白色
        if (whiteFlag == true) {
            
            moving = true;
            colorButton = false;
            scaling = false;
            
            player -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeed, Vec2(RGB->getContentSize().width*0.495,RGB->getContentSize().height*0.44));
            
            auto changeColor = CallFunc::create([&](){
                
                border -> setColor(white);
                border -> setName("white");

                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                moving = false;
                scaling = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            player->runAction(Sequence);
            
        }
        
        
        
        
    }
    
    //MARK:プレイヤーが止まったときに伸縮する動作
    if (colorButton == false && scaling == true) {
        
        scaling = false;
        
        auto small = ScaleTo::create(0.3, 0.8);
        auto big = ScaleTo::create(0.3,1);
        
        auto sequence = Sequence::create(small,big, NULL);
        player-> runAction(RepeatForever::create(sequence));
        
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);

        SimpleAudioEngine::getInstance()->playEffect("cursor1.mp3");

        SimpleAudioEngine::getInstance()->setEffectsVolume(0.6f);


    }
    
    
    
    
    
    
    
    
    
    //フレームのカウント
    //frameCount++;
    //CCLOG("%d",frameCount);
    if(startPattern == true){
        
        startPattern =false;
        
        gameSpeed = 2.5;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);
        
        
    }
    
}



//ランダムパターン
void TitleScene::setPattern1(float time){
    std::string pngCircle;
    std::string pngRing;
    std::string dropName;
    Vec2 dropPosition;
    

        auto rnd = arc4random_uniform(7);
    
        if (rnd == 0) {
            
            pngCircle = "blueRing.png";
            dropName = "blue";
            
        }else if(rnd == 1){
            
            pngCircle = "redRing.png";
            dropName = "red";
            
        }else if(rnd == 2){
            
            pngCircle = "greenRing.png";
            dropName = "green";
            
        }else if(rnd == 3){
            
            pngCircle = "yellowRing.png";
            dropName = "yellow";
            
        }else if (rnd == 4){
            
            pngCircle = "aquaRing.png";
            dropName = "aqua";
            
        }else if (rnd == 5){
            
            pngCircle = "purpleRing.png";
            dropName = "purple";
            
            
        }else if (rnd == 6){
            
            pngCircle = "whiteRing.png";
            dropName = "white";
            
        }


    auto dropCircle = Sprite::create(pngCircle);
    dropCircle -> setPosition(Vec2(selfFrame.width/2,selfFrame.height+dropCircle->getContentSize().height/2));
    
    auto ballMove = MoveTo::create(8, Vec2(selfFrame.width/2,0));
    auto ballRemove = RemoveSelf::create(true);
    
    auto sequence = Sequence::create(ballMove,ballRemove, NULL);
    
    dropCircle->runAction(sequence);
    
    
    //円に名前を設定
    dropCircle -> setName(dropName);
    dropCircle -> setTag(3);
    
    //円に物理体を設定
    auto dropCircleBody = PhysicsBody::createBox(Size(selfFrame.width,10));
    dropCircleBody->setDynamic(true); // 重力の影響を受けない
    dropCircleBody->setCategoryBitmask(0x02);
    
    //0にすることで衝突しない。
    dropCircleBody->setCollisionBitmask(0);
    dropCircleBody->setContactTestBitmask(0x01);
    dropCircle->setPhysicsBody(dropCircleBody);
    
    dropVector.pushBack(dropCircle);
    this -> addChild(dropVector.back());
    
    CCLOG("要素数:%d",(int)dropVector.size());
    test = (int)dropVector.size();
    
    if (score > 800 && gameSpeed == 0.8) {
        
        this->unschedule(schedule_selector(TitleScene::setPattern1));
        
        gameSpeed = 0.6;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);
        
    }else if (score > 500 && gameSpeed == 1.0){
        
        this->unschedule(schedule_selector(TitleScene::setPattern1));

        gameSpeed = 0.8;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);

        
    }else if (score > 300 && gameSpeed == 1.5){
    
        this->unschedule(schedule_selector(TitleScene::setPattern1));
        
        gameSpeed = 1.0;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);

        
    }else if (score > 200 && gameSpeed == 2.0){
        
        this->unschedule(schedule_selector(TitleScene::setPattern1));
        
        gameSpeed = 1.5;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);
        
    }else if (score > 100 && gameSpeed == 2.5){
        
        this->unschedule(schedule_selector(TitleScene::setPattern1));
        
        gameSpeed = 2.0;
        
        this -> schedule(schedule_selector(TitleScene::setPattern1), gameSpeed);
        
    }
    
    
    
    

}
