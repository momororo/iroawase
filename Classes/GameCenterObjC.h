#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GameCenterObjC : NSObject
+(void) loginGameCenter;
+(void) openRanking;
+(void) postHighScore:(NSString*)key score:(int)score;
@end
