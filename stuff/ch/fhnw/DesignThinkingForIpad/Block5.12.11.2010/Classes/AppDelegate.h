//
//  AppDelegate.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ItemViewController.h";


@class SwipeViewController;

@class UINavigationController;

@interface AppDelegate : NSObject <UIApplicationDelegate> 
{
	
    UIWindow *window;
	UINavigationController* _navigationController;
	
	// controllers for the different views:
    SwipeViewController* _swipeView;
	ItemViewController* _itemView;
	
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SwipeViewController* _swipeView;
@property (nonatomic, retain) IBOutlet ItemViewController* _itemView;
@property (nonatomic, retain) IBOutlet UINavigationController* _navigationController;


/**
 * Show the swipe view at the given item index
 * @param iItemIndexToShow int
 */
- (void) showSwipeView:(int) iItemIndexToShow;


/**
 * Initialization function for all view controllers
 */
- (void) initControllers;


/**
 * Implementation of ListViewcontrollerDelegate method for taping on a list item
 * @param iItemIndex int
 * @author Lukas N Mueller
 */
-(void) didSelectItem:(int)iItemIndex;


@end
