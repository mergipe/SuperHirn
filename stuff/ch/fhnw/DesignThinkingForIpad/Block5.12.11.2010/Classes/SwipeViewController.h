//
//  SwipeViewController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "PictureView.h"
#import "BottomNavigationViewController.h"

@interface SwipeViewController : UIViewController<UIScrollViewDelegate, BottomNavigationDelegate> 
{
	IBOutlet UIScrollView *mScrollView;	
	NSMutableArray* allSwipeItems;
	int currentItemInView;
	BottomNavigationViewController *bottomNavViewController;
	NSString* title;
}

/**
 * Set the title of this swipe view
 * @param iTitle NSString*
 */
- (void)setTitle:(NSString*) iTitle;

/**
 * Get the title of this swipe view
 * @return NSString*
 */
- (NSString*)title;


/**
 * Hide the bottom navigation view
 */
- (void)hideBottomNavigation;

/**
 * Show the bottom navigation view
 */
- (void)showBottomNavigation;

/**
 * Centers the swipe view at this index
 * @param iItemIndex int
 * @author Lukas Mueller
 */
-(void) centerAtItem:(int)iItemIndex;

/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)setUpSwipeView;

/**
 * Set up of the bottom navigation view
 * @author Lukas Mueller
 */
- (void)setupBottomNavigationView;

/**
 * Add a new view to the swipe view
 * @param iView UIView*
 * @author Lukas Mueller
 */
-(void)addSwipeItem:(UIView*)iView;

@end
