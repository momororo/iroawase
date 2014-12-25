//
//  NendInterstitialModule.h
//  cocos2dxv32test2
//
//  Created by ADN事業部　 on 2014/07/30.
//
//

#ifndef cocos2dxv32test2_NendInterstitialModule_h
#define cocos2dxv32test2_NendInterstitialModule_h

#include "cocos2d.h"

#define NADInterstitialDelegateLoadResultNotification     "NEND_INTERSTITIAL_DELEGATE_LOAD_RESULT_NOTIFICATION"
#define NADInterstitialDelegateShowResultNotification     "NEND_INTERSTITIAL_DELEGATE_SHOW_RESULT_NOTIFICATION"
#define NADInterstitialDelegateClickResultNotification    "NEND_INTERSTITIAL_DELEGATE_CLICK_RESULT_NOTIFICATION"

typedef enum {
    FINISH_LOAD_AD_INTERSTITIAL,
    FAIL_AD_DOWNLOAD_INTERSTITIAL,
    INVALID_RESPONSE_TYPE_INTERSTITIAL,
    FAIL_AD_REQUEST_INTERSTITIAL,
    FAIL_AD_INCOMPLETE_INTERSTITIAL
} NADInterstitialDelegateLoadResultStatusCode;

typedef enum {
    SHOW_SUCCESS,
    SHOW_ALREADY,
    SHOW_FREQUENCY_NOT_REACHABLE,
    REQUEST_INCOMPLETE,
    LOAD_INCOMPLETE,
    DOWNLOAD_INCOMPLETE
} NADInterstitialDelegateShowResultStatusCode;

typedef enum {
    CLICK_DOWNLOAD,
    CLICK_CLOSE,
    CLICK_EXIT
} NADInterstitialDelegateClickResultStatusCode;

using namespace cocos2d;

class NendInterstitialModule
{
private:
    NendInterstitialModule();
    // シングルトン用クラス保持
    static NendInterstitialModule* m_mySingleton;
    
public:
    //クラス取得
    static NendInterstitialModule* sharedNendInterstitialModule();
    // NADInterstitial生成、API Key, NendIDを設定
    static void createNADInterstitial(char* apiKey, char* spotID);
    // NADInterstitial表示
    static void showNADInterstitialView();
    // NADInterstitial非表示
    static void dismissNADInterstitialView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // NADInterstitial終了時広告（※Androidのみ）
    static void showFinishNADInterstitialView();
#endif
    
    // EventDispatcherでデリゲート結果を通知する
    static void sendEventDispatcherLoadResult(int resultCode);
    static void sendEventDispatcherShowResult(int resultCode);
    static void sendEventDispatcherClickResult(int resultCode);

};

#endif
