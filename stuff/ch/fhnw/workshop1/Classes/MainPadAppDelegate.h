//
//  MainPadAppDelegate.h
//
//

#import <UIKit/UIKit.h>

@class MainViewController;

@interface MainPadAppDelegate : NSObject <UIApplicationDelegate> 
{

	UIWindow *window;
	UINavigationController *mNavController;
	MainViewController *mMainViewController;
	
	NSString *latestIssuePath;
}

@property(nonatomic, retain) IBOutlet UIWindow *window;
@property(nonatomic, retain) NSString *latestIssuePath;

- (void)copySampleRecordToDocuments;
- (NSString *)RecordBasePath;

- (BOOL)displayRecordCovers;

@end

