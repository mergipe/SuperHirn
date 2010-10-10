//
//  WorkshopOne2AppDelegate.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class FullScreenViewController;
@class SwipeViewController;
@class RootViewController;
@class DetailViewController;
@class UITabBarController;
@interface WorkshopOne2AppDelegate : NSObject <UIApplicationDelegate> 
{
	
	UITabBarController* _tabBarController;
    
    UIWindow *window;
    SwipeViewController *_swipeView;
    FullScreenViewController *_fullView;
	DetailViewController *detailViewController;

	/*
	 UISplitViewController *splitViewController;
    RootViewController *rootViewController;
    	 */
	//UINavigationController* _NavigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SwipeViewController* _swipeView;
@property (nonatomic, retain) IBOutlet FullScreenViewController* _fullView;
@property (nonatomic, retain) IBOutlet DetailViewController *detailViewController;


- (void) showFullScreenPictureView:(NSNotification *)notification;
- (void) showPictureSwipeView:(NSNotification *)notification;


/*
@property (nonatomic, retain) IBOutlet UINavigationController* _NavigationController;
@property (nonatomic, retain) IBOutlet UISplitViewController* splitViewController;
@property (nonatomic, retain) IBOutlet RootViewController *rootViewController;
*/
@end
