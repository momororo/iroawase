//
//  TutorialScene.h
//  Shelling
//
//  Created by 新井脩司 on 2014/11/12.
//
//

#ifndef __Shelling__TutorialScene__
#define __Shelling__TutorialScene__

#include <stdio.h>
#include "cocos2d.h"

class TutorialScene : public cocos2d::Layer{

protected:
    
    //1フレームごとに呼ばれる。
    void update(float frame);
    
    
    
    
public:
    
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TutorialScene);
    
    
    //タップイベントの宣言
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    
    //衝突イベント
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    void setRGB();
    void setPlayer();
    
    void setBlueBt();
    void setRedBt();
    void setGreenBt();
    
    void setBlueYajirusi();
    void setGreenYajirusi();
    void setRedYajirusi();
    
    void setSwipe();
    
    void setShooting();
    
    






};
#endif /* defined(__Shelling__TutorialScene__) */
