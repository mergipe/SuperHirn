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

}


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
 * Add a new view to the swipe view
 * @param iView UIView*
 * @author Lukas Mueller
 */
-(void)addSwipeItem:(UIView*)iView;


/**
 * Adjust alls items in the swipe view to the device orientation
 * @author Lukas N Mueller
 */
-(void)adjustSwipeItemsToOrientation;

@end
