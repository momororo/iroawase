#ifndef __JumpingNingjya2__GameCenterBridge__
#define __JumpingNingjya2__GameCenterBridge__

#include <string>

class GameCenterBridge{
public:
    static void loginGameCenter();
    static void openRanking();
    static void postHighScore(std::string key, int score);
};

#endif /* defined(__JumpingNingjya2__GameCenterBridge__) */
