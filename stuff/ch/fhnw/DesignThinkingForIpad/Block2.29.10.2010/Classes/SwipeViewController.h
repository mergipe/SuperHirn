//
//  SwipeViewController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "PictureView.h"


@interface SwipeViewController : UIViewController<UIScrollViewDelegate> 
{
	NSMutableArray* listOfPictures;
	IBOutlet UIScrollView *mScrollView;	
	CGRect itemArea;
}
@end

@interface SwipeViewController()


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
-(void)initImageSwipeViews;


@end
