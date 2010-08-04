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

- (void)copySampleContentToDocuments;
- (NSString *)contentBasePath;

- (BOOL)displayArticleCovers;

@end

