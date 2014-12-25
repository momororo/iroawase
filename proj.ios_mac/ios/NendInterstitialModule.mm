//
//  NendInterstitialModule.m
//  cocos2dxv32test2
//
//  Created by ADN事業部　 on 2014/07/30.
//
//

#import "NendInterstitialModule.h"
#include "AppController.h"
#import "RootViewController.h"
#import "CCEAGLView.h"
#import "NADInterstitial.h"

@interface NadInterstitialDelegate : NSObject<NADInterstitialDelegate>
{
}
@end

@implementation NadInterstitialDelegate

//NADViewDelegateの設定
- (void)didFinishLoadInterstitialAdWithStatus:(NADInterstitialStatusCode)status{
    
    switch ( status ) {
        case SUCCESS:
            NendInterstitialModule::sendEventDispatcherLoadResult(FINISH_LOAD_AD_INTERSTITIAL);
            break;
        case INVALID_RESPONSE_TYPE:
            NendInterstitialModule::sendEventDispatcherLoadResult(INVALID_RESPONSE_TYPE_INTERSTITIAL);
            break;
        case FAILED_AD_REQUEST:
            NendInterstitialModule::sendEventDispatcherLoadResult(FAIL_AD_REQUEST_INTERSTITIAL);
            break;
        case FAILED_AD_DOWNLOAD:
            NendInterstitialModule::sendEventDispatcherLoadResult(FAIL_AD_DOWNLOAD_INTERSTITIAL);
            break;
        default:
            break;
    }

}
- (void)didClickWithType:(NADInterstitialClickType)type{
    
    switch ( type ) {
        case DOWNLOAD:
            // ダウンロードボタンがクリックされました。
            NendInterstitialModule::sendEventDispatcherClickResult(CLICK_DOWNLOAD);
            break;
        case CLOSE:
            // 閉じるボタンあるいは広告範囲外の領域がクリックされました。
            NendInterstitialModule::sendEventDispatcherClickResult(CLICK_CLOSE);
            break;
        default:
            break;
    }

}
@end


//シングルトンの設定
NendInterstitialModule* NendInterstitialModule::m_mySingleton = NULL;
NADInterstitial* interstitial = nil;
NSString* nadInterstitialApiKey;
NSString* nadInterstitialSpotID;

NendInterstitialModule::NendInterstitialModule()
{
    
}

NendInterstitialModule* NendInterstitialModule::sharedNendInterstitialModule(){
    
    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendInterstitialModule();
    }
    
    return m_mySingleton;
}


void NendInterstitialModule::createNADInterstitial(char* apiKey, char* spotID){
    
    if (interstitial) {
        return;
    }
    
    NendInterstitialModule::sharedNendInterstitialModule();
    
    [[NADInterstitial sharedInstance] setDelegate:(id<NADInterstitialDelegate>)[[NadInterstitialDelegate alloc] init]];
    
    nadInterstitialApiKey = [NSString stringWithCString:apiKey encoding:NSUTF8StringEncoding];
    nadInterstitialSpotID = [NSString stringWithCString:spotID encoding:NSUTF8StringEncoding];
    
    [[NADInterstitial sharedInstance] loadAdWithApiKey:nadInterstitialApiKey
                                                spotId:nadInterstitialSpotID];
    
    // 横向きのみのアプリの場合
//    NSArray* array = @[[NSNumber numberWithInteger:UIInterfaceOrientationLandscapeLeft], [NSNumber numberWithInteger:UIInterfaceOrientationLandscapeRight]];
//    [NADInterstitial sharedInstance].supportedOrientations = array;
}


void NendInterstitialModule::dismissNADInterstitialView(){
    [[NADInterstitial sharedInstance] dismissAd];
}

void NendInterstitialModule::showNADInterstitialView(){

    NADInterstitialShowResult result = [[NADInterstitial sharedInstance] showAd];

    switch ( result )
    {
        case AD_SHOW_SUCCESS:
            // 広告の表示に成功しました。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_SUCCESS);
            break;
        case AD_SHOW_ALREADY:
            // 既に広告が表示されています。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_ALREADY);
            break;
        case AD_FREQUENCY_NOT_REACHABLE:
            // 広告のフリークエンシーカウントに達していません。
            NendInterstitialModule::sendEventDispatcherShowResult(SHOW_FREQUENCY_NOT_REACHABLE);
            break;
        case AD_REQUEST_INCOMPLETE:
            // 抽選リクエストに失敗しています。
            NendInterstitialModule::sendEventDispatcherShowResult(REQUEST_INCOMPLETE);
            break;
        case AD_LOAD_INCOMPLETE:
            // 抽選リクエストが実行されていない、もしくは実行中です。
            NendInterstitialModule::sendEventDispatcherShowResult(LOAD_INCOMPLETE);
            break;
        case AD_DOWNLOAD_INCOMPLETE:
            // 広告のダウンロードが完了していません。
            NendInterstitialModule::sendEventDispatcherShowResult(DOWNLOAD_INCOMPLETE);
            break;
    }
}

void NendInterstitialModule::sendEventDispatcherLoadResult(int resultCode){
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateLoadResultNotification,&resultCode);
}

void NendInterstitialModule::sendEventDispatcherShowResult(int resultCode){
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateShowResultNotification,&resultCode);
}

void NendInterstitialModule::sendEventDispatcherClickResult(int resultCode){
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateClickResultNotification,&resultCode);
}

