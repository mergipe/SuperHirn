//
//  SwipeViewController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "PictureView.h"


@interface SwipeViewController : UIViewController 
{
	NSMutableArray* listOfPictures;
	IBOutlet UIScrollView *mScrollView;	
	CGRect itemArea;
}
@end

@interface SwipeViewController()


/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)initImageSwipeViews;


@end
