//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "PictureView.h"


@interface ItemViewController : UIViewController<UIScrollViewDelegate> 
{
	NSMutableArray* listOfPictures;
	IBOutlet UIScrollView *mScrollView;	
	CGRect imageArea;
}
@end

@interface ItemViewController()

/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)initItemView;


@end
