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
@class ItemViewController;

@class UITabBarController;
@class UINavigationController;
@interface WorkshopOne2AppDelegate : NSObject <UIApplicationDelegate> 
{
	
	UITabBarController* _tabBarController;
    UINavigationController* _navigationController;
	
	
    UIWindow *window;
	
    SwipeViewController *_swipeView;
    FullScreenViewController *_fullView;
	ItemViewController *_itemView;
	
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ItemViewController* _itemView;
@property (nonatomic, retain) IBOutlet SwipeViewController* _swipeView;
@property (nonatomic, retain) IBOutlet FullScreenViewController* _fullView;
@property (nonatomic, retain) IBOutlet UINavigationController* _navigationController;
@property (nonatomic, retain) IBOutlet UITabBarController* _tabBarController;

/**
 * Show the full screen picture view
 */
- (void) showFullScreenPictureView:(NSNotification *)notification;

/**
 * Show the swipe picture view
 */
- (void) showPictureSwipeView:(NSNotification *)notification;

/**
 * Initialization function for all view controllers
 */
- (void) initControllers;

/**
 * Set up of a tab bar navigation controller
 */
- (void) setUpTabBarController;

/**
 * Set up of a hirarchical navigation controller
 */
- (void) setUpNavigationController;
@end
