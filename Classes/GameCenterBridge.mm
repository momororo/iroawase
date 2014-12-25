#include "GameCenterBridge.h"
#include "GameCenterObjC.h"

void GameCenterBridge::loginGameCenter(){
    [GameCenterObjC loginGameCenter];
}

void GameCenterBridge::postHighScore(std::string key, int score){
    [GameCenterObjC postHighScore:[[[NSString alloc] initWithCString:key.c_str() encoding:NSUTF8StringEncoding] autorelease] score:score];
}

void GameCenterBridge::openRanking(){
    [GameCenterObjC openRanking];
}
