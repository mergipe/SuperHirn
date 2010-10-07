//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import <UIKit/UIKit.h>



@interface SwipeViewController : UIViewController<UIScrollViewDelegate> 
{
	NSMutableArray* listOfPictures;
	IBOutlet UIScrollView *mScrollView;	
	CGRect imageArea;
}
@end

@interface SwipeViewController()

/**
 * Hide navigation bar
 * @author Lukas Mueller
 */
- (void) hideNavigationBar;

/**
 * Show navigation bar
 * @author Lukas Mueller
 */
- (void) showNavigationBar;

/**
 * Overlay the background image over the default splash screen to smooth the transition
 * @author Lukas Mueller
 */
- (void)showThommenBackgroundImageOnStart;


/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)initImageSwipeViews;


@end
