//
//  WorkshopOne2AppDelegate.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>


@class FullScreenViewController;
@class SwipeViewController;
@class ItemViewController;
@class UITabBarController;
//@class UINavigationController;

@interface WorkshopOne2AppDelegate : NSObject <UIApplicationDelegate> 
{
	
    UIWindow *window;
	UITabBarController* _tabBarController;
	// UINavigationController* _navigationController;
	

	// controllers for the different app views:
    SwipeViewController *_swipeView;
    FullScreenViewController *_fullView;
	ItemViewController *_itemView;
	
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ItemViewController* _itemView;
@property (nonatomic, retain) IBOutlet SwipeViewController* _swipeView;
@property (nonatomic, retain) IBOutlet FullScreenViewController* _fullView;
@property (nonatomic, retain) IBOutlet UITabBarController* _tabBarController;

// @property (nonatomic, retain) IBOutlet UINavigationController* _navigationController;

/**
 * Show the full screen item view
 */
- (void) showFullScreenItemView:(NSNotification *)notification;

/**
 * Show the swipe view
 */
- (void) showSwipeView:(NSNotification *)notification;

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
//- (void) setUpNavigationController;

@end
