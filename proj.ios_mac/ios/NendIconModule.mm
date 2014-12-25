//
//  NendIconModule.mm
//
//  Created by F@N Communications, Inc.
//
//

#include "NendIconModule.h"
#include "AppController.h"
#import "RootViewController.h"
#import "CCEAGLView.h"
#import "NADIconLoader.h"

#define ICON_DISPLAY_COUNT 4

@interface NadIconLoaderDelegate : NSObject<NADIconLoaderDelegate>
{
}
@end

@implementation NadIconLoaderDelegate

//NADViewDelegateの設定
/**
 CCNotificationCenterのcocos2d-x-v3？から非推奨のためNotificationCenterに置き換え
 */
-(void)nadIconLoaderDidFinishLoad:(NADIconLoader *)iconLoader{
    //初回ロード終了後の処理を記述
    NendIconModule::sendEventDispatcher(FINISH_LOAD_AD_ICON);
}

-(void)nadIconLoaderDidReceiveAd:(NADIconLoader *)iconLoader nadIconView:(NADIconView *)nadIconView{
    //ロード終了後処理を記述
    NendIconModule::sendEventDispatcher(RECEIVE_AD_ICON);
}

-(void)nadIconLoaderDidFailToReceiveAd:(NADIconLoader *)iconLoader nadIconView:(NADIconView *)nadIconView{
    //ロード失敗の処理を記述
    NendIconModule::sendEventDispatcher(FAIL_TO_RECEIVE_AD_ICON);
}

-(void)nadIconLoaderDidClickAd:(NADIconLoader *)iconLoader nadIconView:(NADIconView *)nadIconView{
    //広告クリック時の処理を記述
    NendIconModule::sendEventDispatcher(CLICK_AD_ICON);
}

@end



//シングルトンの設定
NendIconModule* NendIconModule::m_mySingleton = NULL;
NADIconLoader* iconLoader = nil;
NSMutableArray* iconViewArray = nil;
NSString* nadIconApiKey;
NSString* nadIconSpotID;


NendIconModule::NendIconModule()
{
    
}

NendIconModule* NendIconModule::sharedNendIconModule(){
    
    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendIconModule();
    }
    
    return m_mySingleton;
}

//NADIconLoader生成
void NendIconModule::createNADIconLoader(char* apiKey, char* spotID){
    if (iconLoader) {
        return;
    }
    
    NendIconModule::sharedNendIconModule();

    iconLoader = [[NADIconLoader alloc] init];
    iconViewArray = [[NSMutableArray alloc] init];
    
    nadIconApiKey = [NSString stringWithCString:apiKey encoding:NSUTF8StringEncoding];
    nadIconSpotID = [NSString stringWithCString:spotID encoding:NSUTF8StringEncoding];
    
    //NADIconLoaderDelegateクラスを生成
    iconLoader.delegate = (id<NADIconLoaderDelegate>)[[NadIconLoaderDelegate alloc] init];    
    
}

//NADIconView生成、API Key, NendIDを設定、位置設定
void NendIconModule::createNADIconView(cocos2d::Point pos){

    //cocos2d-x上の座標をiOSの座標に変換
    //cocos2d-xで管理するwindowサイズを取得
    cocos2d::Size size = Director::getInstance()->getWinSize();
    
    //座標を割合で取得
    cocos2d::Point pointRate;
    pointRate.x = (pos.x / size.width);
    pointRate.y = (pos.y / size.height);
    
    //iOSで管理するViewのサイズを取得
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *baseView = (CCEAGLView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;
    
    //高さをiOS基準に変換する
    CGPoint newPosition;
    newPosition.x = mainViewRect.size.width * pointRate.x;
    newPosition.y = mainViewRect.size.height - (mainViewRect.size.height * pointRate.y);
    
    NADIconView* iconView = [[NADIconView alloc] initWithFrame:CGRectMake(newPosition.x, newPosition.y, NAD_ICONVIEW_SIZE_75x75.width, NAD_ICONVIEW_SIZE_75x75.height)];
    
    AppController* app = [UIApplication sharedApplication].delegate;
    RootViewController* viewController = app.viewController;
    [viewController.view addSubview:iconView];
    
    [iconLoader addIconView:iconView];
    [iconViewArray addObject:iconView];
}

void NendIconModule::createNADIconViewBottom(int iconCount){

    //画面下部にアイコンを並べて表示する
    
    if (iconCount > 6) {
        iconCount = 6;
    }
    
    int marginIconPositionX = 0;
    if (iconCount % 2 != 0) {
        marginIconPositionX = NAD_ICONVIEW_SIZE_75x75.width/2;
    }

    //Windowサイズを取得
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *baseView = (CCEAGLView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;


    AppController* app = [UIApplication sharedApplication].delegate;
    RootViewController* viewController = app.viewController;

    for (int i = (int)[iconViewArray count]; i < iconCount; i++) {
        NADIconView* iconView = [[NADIconView alloc] init];
        //中央に並べる
        [iconView setFrame:CGRectMake(
                                       (mainViewRect.size.width /2) - marginIconPositionX + ((NAD_ICONVIEW_SIZE_75x75.width * (i -(iconCount /2) ))),
                                      mainViewRect.size.height - (NAD_ICONVIEW_SIZE_75x75.height ),
                                      iconView.frame.size.width,
                                      iconView.frame.size.height)];
        
        [viewController.view addSubview:iconView];

        [iconLoader addIconView:iconView];
        [iconViewArray addObject:iconView];

    }
    
}

void NendIconModule::createNADIconViewTop(int iconCount){
    
    //画面上部にアイコンを並べて表示する
    
    if (iconCount > 6) {
        iconCount = 6;
    }
    
    int marginIconPositionX = 0;
    if (iconCount % 2 != 0) {
        marginIconPositionX = NAD_ICONVIEW_SIZE_75x75.width/2;
    }
    
    //Windowサイズを取得
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *baseView = (CCEAGLView *) view->getEAGLView();
    CGRect mainViewRect = baseView.bounds;
    
    
    AppController* app = [UIApplication sharedApplication].delegate;
    RootViewController* viewController = app.viewController;

    for (int i = (int)[iconViewArray count]; i < iconCount; i++) {
        NADIconView* iconView = [[NADIconView alloc] init];
        //中央に並べる
        [iconView setFrame:CGRectMake(
                                      (mainViewRect.size.width /2) - marginIconPositionX + ((NAD_ICONVIEW_SIZE_75x75.width * (i -(iconCount /2) ))),
                                      0,
                                      iconView.frame.size.width,
                                      iconView.frame.size.height)];
        
        [viewController.view addSubview:iconView];

        [iconLoader addIconView:iconView];
        [iconViewArray addObject:iconView];
        
    }
    
}

void NendIconModule::createNADIconViewBottom(){
    createNADIconViewBottom(ICON_DISPLAY_COUNT);
}

void NendIconModule::createNADIconViewTop(){
    createNADIconViewTop(ICON_DISPLAY_COUNT);
}

void NendIconModule::hideNADIconView(){
    for (NADIconView* iconView in iconViewArray) {
        [iconView setHidden:YES];
    }
}

void NendIconModule::showNADIconView(){
    for (NADIconView* iconView in iconViewArray) {
        [iconView setHidden:NO];
    }
}

void NendIconModule::load(){
    [iconLoader setNendID:nadIconApiKey spotID:nadIconSpotID];
    [iconLoader load];
}


void NendIconModule::pause(){
    [iconLoader pause];
}

void NendIconModule::resume(){
    [iconLoader resume];
}

void NendIconModule::release(){
    NendIconModule::pause();
    [iconLoader.delegate release];
    iconLoader.delegate = nil;
    [iconLoader release];
    iconLoader = nil;
    
    for (NADIconView* iconView in iconViewArray) {
        [iconView removeFromSuperview];
        [iconView release];
    }

    [iconViewArray removeAllObjects];
    [iconViewArray release];
    iconViewArray = nil;
}

void NendIconModule::sendEventDispatcher(int resultCode){
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADIconLoaderDelegateNotification,&resultCode);
}
