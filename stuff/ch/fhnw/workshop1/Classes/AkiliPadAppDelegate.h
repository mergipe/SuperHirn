//
//  AkiliPadAppDelegate.h
//  AkiliPad
//
//  Created by Mithin on 21/04/10.
//  Copyright Techtinium Corporation 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;

@interface AkiliPadAppDelegate : NSObject <UIApplicationDelegate> {
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

