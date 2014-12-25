//
//  TutorialScene.cpp
//  Shelling
//
//  Created by 新井脩司 on 2014/11/12.
//
//

#include "TutorialScene.h"
#include "TitleScene.h"
#include <string.h>

#define selfFrame Director::getInstance() -> getWinSize()
#define KODOMO_FONT "KodomoRounded"

cocos2d::Sprite *B;

USING_NS_CC;
using namespace CocosDenshion;





//ゲームボタン
Sprite *blueButtonT;
Sprite *redButtonT;
Sprite *greenButtonT;


Sprite *playerT;

//操作ボタン
Sprite *RGBT;

//操作ボタンのダミー
Sprite *pRGBT;

//境界線
Sprite *borderT;

//落下する輪

//タッチポイントの保存
Point beganPointT;

// MARK:タッチ範囲スプライトの透明度の設定
int clearColorT = 0;
float playScaleT = 0.7;
float playerSpeedT = 0.1;


//タッチムーブフラグ
bool touchMoveT;

bool colorButtonT;
//RGBプレイヤーの動き
bool movingT;
//RGBプレイヤーの伸縮運動
bool scalingT;

//色のフラグ
bool blueFlagT;
bool redFlagT;
bool greenFlagT;
bool aquaFlagT;
bool yellowFlagT;
bool purpleFlagT;
bool whiteFlagT;

bool contactT;


//色判別用
auto pblueT = Color4B(42, 127, 255,255);
auto predT = Color4B(255, 85, 85,255);
auto pgreenT = Color4B(85, 255, 85,255);

auto blueT = Color3B(42, 127, 255);
auto redT = Color3B(255, 85, 85);
auto greenT = Color3B(85, 255, 85);
auto aquaT = Color3B(42, 212, 255);
auto purpleT = Color3B(229, 128, 255);
auto yellowT = Color3B(255, 221, 85);
auto whiteT = Color3B(255, 255, 255);


bool tutorialFlag;
bool tutorialTouchFlag;

Label *tutorialLabel;

int count;

Sprite *blueYajirusi;
Sprite *redYajirusi;
Sprite *greenYajirusi;
Sprite *swipeYajirusi;

bool touchBlue;
bool touchRed;
bool touchGreen;

Sprite *tutorialRing;







Scene *TutorialScene::createScene(){
    
    auto scene = Scene::createWithPhysics();
    auto layer = TutorialScene::create();
    scene -> addChild(layer);
    
    auto world = scene -> getPhysicsWorld();
    Vect gravity;
    gravity.setPoint(0, -50);
    world -> setGravity(gravity);
    
    
    
    return scene;
}

bool TutorialScene::init(){

    if (!Layer::init()) {
        return false;
    }
    
    this->scheduleUpdate();

    
    auto bgColor = LayerColor::create(Color4B(255, 246, 214, 255));
    this -> addChild(bgColor);

    tutorialLabel = Label::createWithSystemFont("赤・青・緑の３色を\nタップすることで,\n色をかえることができます。",KODOMO_FONT , 40);
    tutorialLabel -> setColor(Color3B::GRAY);
    tutorialLabel -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*2/3));
    this -> addChild(tutorialLabel,10000);
    
    
    
    tutorialFlag = true;
    tutorialTouchFlag = false;
    touchBlue = false;
    touchRed  = false;
    touchGreen = false;
    count = 0;
    
  
    
  
    setRGB();
    setPlayer();
    
    setBlueYajirusi();
    setGreenYajirusi();
    setRedYajirusi();
    
    
    
    
    
    
    
    /**************　タッチイベント設定  ******************/
 
    //シングルタップ用リスナーを用意する
    auto listener = EventListenerTouchOneByOne::create();
    
    
    //各イベントの割り当て
    listener -> onTouchBegan = CC_CALLBACK_2(TutorialScene::onTouchBegan,this);
    listener -> onTouchMoved = CC_CALLBACK_2(TutorialScene::onTouchMoved,this);
    listener -> onTouchEnded = CC_CALLBACK_2(TutorialScene::onTouchEnded,this);
    
    
    //イベントディスパッチャようにリスナーを追加する
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
   
    /*************　　タッチイベント設定  終 ****************/
    
    /*************　  衝突イベント設定   ****************/
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener -> onContactBegin = CC_CALLBACK_1(TutorialScene::onContactBegin,this);
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(contactListener,this);
    
    /*************　  衝突イベント設定  終 ****************/




    return true;

}

bool TutorialScene::onTouchBegan(Touch *touch, Event *event){
    
    
    //タップ開始時の処理
    CCLOG("touchBegan");
    
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);
    
    beganPointT = touchPoint;
    
    return true;
}

//MARK::touchEnd
void TutorialScene::onTouchEnded(Touch *touch, cocos2d::Event *event){
    
    //タップ終了時
    CCLOG("touchEnded");
    
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);

    
    if (tutorialFlag == true) {
        
        
        if (count == 0) {
            
            std::string str = StringUtils::format("青色をタップすると・・・");
            tutorialLabel -> setString(str);
            
            redYajirusi -> runAction(RemoveSelf::create(true));
            greenYajirusi -> runAction(RemoveSelf::create(true));
            
            setBlueBt();
            touchBlue = true;
            tutorialTouchFlag = true;
            tutorialFlag = false;
            
            count = 1;
            
            //青ボタン上で押すと次のフェーズに入ってしまうため、強制的に出る
            return;
            
        }else if (count == 2){
            
            std::string str = StringUtils::format("次は緑をタップしてみましょう");
            tutorialLabel -> setString(str);
            
            setGreenYajirusi();
            setGreenBt();
            touchGreen = true;
            tutorialFlag = false;
            tutorialTouchFlag = true;
            
            count = 3;
            
            return;
            
        }else if(count == 4){
            
            tutorialRing = Sprite::create("yellowRing.png");
            tutorialRing -> setPosition(Vec2(selfFrame.width/2,selfFrame.height+tutorialRing->getContentSize().height/2));
            
            auto ringMove = MoveTo::create(3, Vec2(selfFrame.width/2,selfFrame.height*3/4));
            
            tutorialRing -> runAction(ringMove);//seq);
            
            
            //円に名前を設定
            tutorialRing -> setName("yellow");
            tutorialRing -> setTag(3);
            
            //円に物理体を設定
            auto dropCircleBody = PhysicsBody::createBox(Size(selfFrame.width,10));
            dropCircleBody->setDynamic(false); // 重力の影響を受けない
            dropCircleBody->setCategoryBitmask(0x02);
            
            //0にすることで衝突しない。
            dropCircleBody -> setCollisionBitmask(0);
            dropCircleBody -> setContactTestBitmask(0x01);
            tutorialRing -> setPhysicsBody(dropCircleBody);
            
            this-> addChild(tutorialRing);
            
            
            std::string str = StringUtils::format("黄色の円が出てきた！\n\nまわりの色を、\n円の色にあわせよう!");
            tutorialLabel -> setString(str);
            
            count = 5;
            
            return;
        
        }else if (count == 5){
            
            std::string str = StringUtils::format("まずは緑をタップ！");
            tutorialLabel -> setString(str);
            
            setGreenYajirusi();
            setGreenBt();
            touchGreen = true;
            tutorialFlag = false;
            tutorialTouchFlag = true;
            
            count = 6;
            
            return;
        }else if(count ==8){
            
            std::string str = StringUtils::format("同じ色どうしだと・・・");
            tutorialLabel -> setString(str);
            
            count = 9;
            
            return;
        
        
        }else if (count == 9){
            
            tutorialRing -> getPhysicsBody()-> setDynamic(true);
            
            auto ringMove = MoveTo::create(4, Vec2(selfFrame.width/2,0));
            
            tutorialRing -> runAction(ringMove);
            
            count = 10;
            
            return;
        
        }else if(count == 11){
            
            tutorialRing = Sprite::create("yellowRing.png");
            tutorialRing -> setPosition(Vec2(selfFrame.width/2,selfFrame.height+tutorialRing->getContentSize().height/2));
            
            auto ringMove = MoveTo::create(2, Vec2(selfFrame.width/2,selfFrame.height*3/4));
            
            tutorialRing -> runAction(ringMove);//seq);
            
            
            //円に名前を設定
            tutorialRing -> setName("yellow");
            tutorialRing -> setTag(3);
            
            //円に物理体を設定
            auto dropCircleBody = PhysicsBody::createBox(Size(selfFrame.width,10));
            dropCircleBody->setDynamic(false); // 重力の影響を受けない
            dropCircleBody->setCategoryBitmask(0x02);
            
            //0にすることで衝突しない。
            dropCircleBody -> setCollisionBitmask(0);
            dropCircleBody -> setContactTestBitmask(0x01);
            tutorialRing -> setPhysicsBody(dropCircleBody);
            
            this-> addChild(tutorialRing);
            
            
            std::string str = StringUtils::format("ほかにも消し方があるよ。");
            tutorialLabel -> setString(str);
            
            count = 12;
            
            return;
        
        }else if (count == 12){
            
            std::string str = StringUtils::format("円のまん中を下に引くと・・・");
            tutorialLabel -> setString(str);
            
            tutorialFlag = false;
            tutorialTouchFlag = true;
            
            setSwipe();
            
            count = 13;
            
            return;
        
        }else if (count == 14){
            
            std::string str = StringUtils::format("ボールで消すとポイント10倍！\nじゃんじゃん消して、\nがんがんスコアをかせごう！！");
            tutorialLabel -> setString(str);
            
            count = 15;
            
            return;
            
        }else if(count == 15){
            
            //アニメーション付き
            float duration = 2.0f;  //開始→終了にかける時間
            Scene* nextScene = CCTransitionFade::create(duration, TitleScene::createScene());
            
            Director::getInstance()->replaceScene(nextScene);
        }

    }
    
    
    /******************* ゲームボタン設定 ************************/
    
    //青色を押したときの動作
    //ボタンがないとき
    if (tutorialTouchFlag == true) {
        
        Point touchRGB = RGBT -> convertToNodeSpace(touchPoint);
        
        
        if (touchMoveT == false) {
            
            //MARK:青ボタンをタップしたとき
            if(touchBlue == true && blueButtonT -> getBoundingBox().containsPoint(touchRGB)){
                
                if(borderT && movingT == false){
                    
                    //背景が赤のとき
                    if (redFlagT == true) {
                        
                        //紫に変更
                        redFlagT = false;
                        purpleFlagT = true;
                        colorButtonT = true;
                        
                        //背景が緑のとき
                    }else if (greenFlagT == true){
                        
                        //水色に変更
                        greenFlagT = false;
                        aquaFlagT = true;
                        colorButtonT = true;
                        
                        //背景が黄色のとき
                    }else if (yellowFlagT == true){
                        
                        //白に変更
                        yellowFlagT = false;
                        whiteFlagT = true;
                        colorButtonT = true;
                        
                        //背景が紫のとき
                    }else if (purpleFlagT == true || aquaFlagT == true || whiteFlagT == true){
                        
                        blueFlagT = true;
                        purpleFlagT = false;
                        aquaFlagT = false;
                        whiteFlagT = false;
                        colorButtonT = true;
                        
                        CCLOG("colorButtonT はトゥルー");
                        
                    }else if(blueFlagT == true){
                        
                        return;
                        
                    }
                    
                    return;
                }
                
                return;
            }
            
            //MARK:赤のボタン設定
            if (touchRed == true && redButtonT -> getBoundingBox().containsPoint(touchRGB)){
                
                if(borderT && movingT == false){
                    
                    //青色のとき
                    if (blueFlagT == true) {
                        
                        blueFlagT = false;
                        purpleFlagT = true;
                        colorButtonT = true;
                        
                    }else if (greenFlagT == true){
                        
                        greenFlagT = false;
                        yellowFlagT = true;
                        colorButtonT = true;
                        
                    }else if (aquaFlagT == true){
                        
                        aquaFlagT = false;
                        whiteFlagT = true;
                        colorButtonT = true;
                        
                    }else if (purpleFlagT == true || yellowFlagT ==true || whiteFlagT == true){
                        
                        purpleFlagT = false;
                        yellowFlagT = false;
                        whiteFlagT = false;
                        redFlagT = true;
                        colorButtonT = true;
                        
                    }else if(redFlagT == true){
                        
                        return;
                        
                    }
                    
                    return;
                    
                }
                
                return;
                
            }
            
            //MARK:緑ボタン
            if (touchGreen == true && greenButtonT-> getBoundingBox().containsPoint(touchRGB)){
                
                if(borderT && movingT == false){
                    
                    if (blueFlagT == true) {
                        
                        blueFlagT = false;
                        aquaFlagT = true;
                        colorButtonT = true;
                        
                    }else if (redFlagT == true){
                        
                        redFlagT = false;
                        yellowFlagT = true;
                        colorButtonT = true;
                        
                    }else if (purpleFlagT == true){
                        
                        purpleFlagT = false;
                        whiteFlagT = true;
                        colorButtonT = true;
                        
                    }else if (aquaFlagT == true || yellowFlagT == true || whiteFlagT == true){
                        
                        aquaFlagT = false;
                        yellowFlagT = false;
                        whiteFlagT = false;
                        greenFlagT = true;
                        colorButtonT = true;
                        
                    }else if(greenFlagT == true){
                        
                        return;
                        
                    }
                    
                    return;
                    
                }
                
                return;
                
            }
            
            return;
            
        }else{
            
            //MARK:攻撃開始
            touchMoveT = false;
            RGBT -> setPosition(Vec2(pRGBT->getPosition().x,pRGBT->getPosition().y));
            setShooting();
            
        }
        
        return;
        
    }
    
    /******************************** ボタン設定 終 **********************************/
    
    
    
    
    



}


void TutorialScene::onTouchMoved(Touch *touch, cocos2d::Event *event){
    
    
    //スワイプ中の処理
    CCLOG("touchMoved");
    Point touchPoint = Vec2(touch->getLocation().x,touch->getLocation().y);
   
    if (tutorialTouchFlag == true && count == 13) {
        
        if (RGBT -> getBoundingBox().containsPoint(touchPoint)) {
            if ((beganPointT.y-touchPoint.y > 0)){
                
                RGBT -> setPosition(Vec2(pRGBT->getPosition().x,pRGBT->getPosition().y-50));
                touchMoveT = true;
                
            }else{
                
                return;
                
            }
            
            return;
            
        }
    }

}


// MARK:操作ボタンの設定
void TutorialScene::setRGB(){
    
    RGBT = Sprite::create("RGB.png");
    RGBT -> setPosition(Vec2(selfFrame.width/2, selfFrame.height*0.26));
    RGBT -> setScale(playScaleT);
    this -> addChild(RGBT,100);
    
    pRGBT = Sprite::create();
    pRGBT -> setTextureRect(Rect(0, 0, 10, 10));
    pRGBT -> setPosition(Vec2(selfFrame.width/2,selfFrame.height*0.26));
    pRGBT -> setColor(Color3B::BLACK);
    pRGBT -> setOpacity(clearColorT);
    this -> addChild(pRGBT);
    
    //境界線
    borderT = Sprite::create("ring.png");
    borderT -> setPosition(Vec2(RGBT->getContentSize().width/2,RGBT->getContentSize().height/2-20));
    borderT -> setColor(Color3B::WHITE);
    //RGB-> addChild(border);
    
    borderT -> setName("white");
    borderT -> setTag(2);
    
    //物理体の生成
    auto borderBody = PhysicsBody::createCircle(borderT->getContentSize().width/2);
    borderBody->setDynamic(false); // 重力の影響を受けない
    borderBody->setEnable(true);
    
    borderBody->setCategoryBitmask(0x01);
    borderBody->setCollisionBitmask(0);
    borderBody->setContactTestBitmask(0x02);
    
    borderT->setPhysicsBody(borderBody);
    
    RGBT ->addChild(borderT);
    
    
    movingT = false;
    whiteFlagT = true;

}

// MARK:プレイヤーの作成
void TutorialScene::setPlayer(){
    
    playerT = Sprite::create("player.png");
    playerT -> setPosition(Vec2(RGBT->getContentSize().width/2,RGBT->getContentSize().height/2-25));
    playerT -> setScale(playScaleT);
    RGBT -> addChild(playerT,1000);
    
}

void TutorialScene::setBlueBt(){
    
    //青ボタン
    blueButtonT = Sprite::create();
    blueButtonT -> setTextureRect(Rect(0,0,310,220));
    blueButtonT -> setPosition(Vec2(RGBT->getContentSize().width/2, RGBT->getContentSize().height*3/4+30));
    blueButtonT -> setColor(Color3B::BLUE);
    blueButtonT -> setOpacity(clearColorT);
    RGBT -> addChild(blueButtonT,200);

}

void TutorialScene::setRedBt(){
    
    //赤ボタン
    redButtonT = Sprite::create();
    redButtonT -> setTextureRect(Rect(0,0,310,220));
    redButtonT -> setPosition(Vec2(RGBT->getContentSize().width*4/5, RGBT->getContentSize().height/5+30));
    redButtonT -> setColor(Color3B::RED);
    redButtonT -> setRotation(-55);
    redButtonT -> setOpacity(clearColorT);
    RGBT -> addChild(redButtonT,200);

}

void TutorialScene::setGreenBt(){
    
    //緑ボタン
    greenButtonT = Sprite::create();
    greenButtonT -> setTextureRect(Rect(0,0,310,220));
    greenButtonT -> setPosition(Vec2(RGBT->getContentSize().width/5, RGBT->getContentSize().height/5+30));
    greenButtonT -> setColor(Color3B::GREEN);
    greenButtonT -> setRotation(55);
    greenButtonT -> setOpacity(clearColorT);
    RGBT -> addChild(greenButtonT,200);

}



void TutorialScene::setBlueYajirusi(){
    
    blueYajirusi = Sprite::create("yajirusi.png");
    blueYajirusi -> setPosition(Vec2(RGBT->getContentSize().width/2, RGBT->getContentSize().height*3/4+100));
    blueYajirusi -> setOpacity(155);
    RGBT -> addChild(blueYajirusi,200);
    
    auto blink = Blink::create(1, 1);
    
    auto repeat = RepeatForever::create(blink);
    
    blueYajirusi -> runAction(repeat);

}

void TutorialScene::setRedYajirusi(){
    
    redYajirusi = Sprite::create("yajirusi.png");
    redYajirusi -> setPosition(Vec2(RGBT->getContentSize().width*4/5, RGBT->getContentSize().height/5+100));
    redYajirusi -> setOpacity(155);
    RGBT -> addChild(redYajirusi,200);

    auto blink = Blink::create(1, 1);
    
    auto repeat = RepeatForever::create(blink);
    
    redYajirusi -> runAction(repeat);
    

}

void TutorialScene::setGreenYajirusi(){
    
    greenYajirusi = Sprite::create("yajirusi.png");
    greenYajirusi -> setPosition(Vec2(RGBT->getContentSize().width/5, RGBT->getContentSize().height/5+100));
    greenYajirusi -> setOpacity(155);
    RGBT -> addChild(greenYajirusi,200);

    auto blink = Blink::create(1, 1);
    
    auto repeat = RepeatForever::create(blink);
    
    greenYajirusi -> runAction(repeat);
}

void TutorialScene::setSwipe(){
    
    swipeYajirusi = Sprite::create("yajirusi.png");
    swipeYajirusi -> setPosition(Vec2(RGBT->getContentSize().width/2, RGBT->getContentSize().height/2-50));
    //swipeYajirusi -> setOpacity(155);
    RGBT -> addChild(swipeYajirusi,200);
    
    auto move = MoveTo::create(2, Vec2(RGBT->getContentSize().width/2,0));
    auto fade = FadeOut::create(2);
    
    auto spawn = Spawn::create(move,fade, NULL);
    
    auto fadeIn = FadeIn::create(0);
    auto back = MoveTo::create(0, Vec2(RGBT->getContentSize().width/2, RGBT->getContentSize().height/2-50));
    
    auto reback = Spawn::create(fadeIn,back, NULL);
    
    auto seq = Sequence::create(spawn,reback, NULL);
    
    auto repeat = RepeatForever::create(seq);
    
    swipeYajirusi -> runAction(repeat);

}


//MARK:射撃
void TutorialScene::setShooting(){
    
    tutorialTouchFlag = false;
    tutorialFlag = true;
    
    
    
    auto ball = Sprite::create("whiteBall.png");
    ball -> setPosition(selfFrame.width/2,pRGBT->getPosition().y + RGBT->getContentSize().height/2);
    ball -> setColor(borderT->getColor());
    ball -> setScale(0.5);
    
    ball ->setTag(1);
    ball -> setName(borderT->getName());
    
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
    
    tutorialRing -> getPhysicsBody() -> setDynamic(true);
    
    
    
    
}




#pragma mark-
#pragma mark 衝突判定
bool TutorialScene::onContactBegin(cocos2d::PhysicsContact& contact){
    
    
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
                
                contactT = true;
                
                
                
                
            }else if (nodeB->getTag() == 2){
                
                circle->stopAllActions();
                
                circle->getPhysicsBody()->removeFromWorld();
                
                auto scale = ScaleTo::create(1, 1.5);
                
                auto fadeOut = FadeOut::create(1);
                
                auto remove = RemoveSelf::create(true);
                
                auto spawn = Spawn::create(scale,fadeOut, NULL);
                
                auto sequence = Sequence::create(spawn,remove, NULL);
                
                circle-> runAction(sequence);
                
                contactT = true;
       
                
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
                
                contactT = true;
                
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
                
                contactT = true;
                
                
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
                
                contactT = true;
                
               
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
           
                contactT = true;
                
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
#pragma mark:フレーム管理

void TutorialScene::update( float frame )
{
    
    //MARK:プレイヤーの移動
    
    if (colorButtonT == true) {
        
        CCLOG("はいったおー");

        
        //青
        if (blueFlagT == true) {
            
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width/2, RGBT->getContentSize().height*5/6));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(blueT);
                borderT -> setName("blue");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
            
        }
        
        //赤
        if (redFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*4/5, RGBT->getContentSize().height*1/5));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(redT);
                borderT -> setName("red");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
            
        }
        
        //緑
        if (greenFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*1/5, RGBT->getContentSize().height*1/5));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(greenT);
                borderT -> setName("green");
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
        }
        
        
        
        //水色
        if (aquaFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*0.32, RGBT->getContentSize().height*0.55));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(aquaT);
                borderT -> setName("aqua");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
        }
        
        //紫色
        if (purpleFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*0.67, RGBT->getContentSize().height*0.55));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(purpleT);
                borderT -> setName("purple");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
        }
        
        //黄色
        if (yellowFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*0.495, RGBT->getContentSize().height*0.25));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(yellowT);
                borderT -> setName("yellow");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT->runAction(Sequence);
            
        }
        
        //白色
        if (whiteFlagT == true) {
            
            movingT = true;
            colorButtonT = false;
            scalingT = false;
            
            playerT -> stopAllActions();
            
            auto movePlayer = MoveTo::create(playerSpeedT, Vec2(RGBT->getContentSize().width*0.495,RGBT->getContentSize().height*0.44));
            
            auto changeColor = CallFunc::create([&](){
                
                borderT -> setColor(whiteT);
                borderT -> setName("white");
                
                
            });
            
            auto movePlayerFlag = CallFunc::create([&](){
                
                movingT = false;
                scalingT = true;
                
            });
            
            auto Sequence = Sequence::create(movePlayer,changeColor,movePlayerFlag, NULL);
            
            playerT -> runAction(Sequence);
            
        }
        
        
        
        
    }
    
    //MARK:プレイヤーが止まったときに伸縮する動作
    if (colorButtonT == false && scalingT == true) {
        
        scalingT = false;
        
        auto small = ScaleTo::create(0.3, 0.8);
        auto big = ScaleTo::create(0.3,1);
        
        auto sequence = Sequence::create(small,big, NULL);
        playerT-> runAction(RepeatForever::create(sequence));
        
    }
    
    //MARK:動的にチュートリアルを観察
    if (count == 1 && blueFlagT == true) {
        
        count = 2;
        touchBlue = false;
        tutorialTouchFlag = false;
        
        tutorialFlag = true;

        
        std::string str = StringUtils::format("まわりの色が青になります！");
        tutorialLabel -> setString(str);
        
        blueButtonT -> removeFromParent();
        blueYajirusi -> removeFromParent();
        
    }else if(count == 3 && aquaFlagT == true){
        
        count = 4;
        touchGreen = false;
        tutorialTouchFlag = false;
        
        tutorialFlag = true;
        
        std::string str = StringUtils::format("青と緑がまざり、\n水色になりました！");
        tutorialLabel -> setString(str);
        
        greenButtonT -> removeFromParent();
        greenYajirusi -> removeFromParent();
        
    }else if (count == 6 && greenFlagT == true){
        
        
        count = 7;
        touchGreen = false;
        
        std::string str = StringUtils::format("緑になったね！\n次は赤をタップ！");
        tutorialLabel -> setString(str);
        
        greenButtonT -> removeFromParent();
        greenYajirusi -> removeFromParent();
        
        touchRed = true;
        setRedBt();
        setRedYajirusi();

    }else if (count == 7 && yellowFlagT == true){
        
        count = 8;
        touchRed = false;
        tutorialTouchFlag = false;
        
        tutorialFlag = true;
        
        std::string str = StringUtils::format("緑と赤がまざり、\n黄色になりました！");
        tutorialLabel -> setString(str);
        
        redButtonT -> removeFromParent();
        redYajirusi -> removeFromParent();
    
    
    }else if (count == 10 && contactT == true){
        
        contactT = false;
        
        std::string str = StringUtils::format("きれいに消えました！\n\nちがう色どうしだと、\nゲームオーバーに...");
        tutorialLabel -> setString(str);
        
        count = 11;
        
        CCLOG("はいったよーわいわい");
    }else if (count == 13 && contactT == true){
        
        contactT = false;
        
        swipeYajirusi -> removeFromParent();
        
        std::string str = StringUtils::format("きれいに消えました！\n\nちがう色どうしだと、\n消えないので注意！");
        tutorialLabel -> setString(str);
        
        count  = 14;
    
    }
    
    
    
    
}










