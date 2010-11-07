//
//  ItemViewController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "TapIcon.h"

#import <UIKit/UIKit.h>
#import "PictureView.h"



@interface ItemViewController : UIViewController<UIScrollViewDelegate, ItemDelegate> 
{
	CGRect itemArea;
}
@end

@interface ItemViewController()

/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)initItemView;


-(void)tapOnItem:(int)iItemIndex;
@end
