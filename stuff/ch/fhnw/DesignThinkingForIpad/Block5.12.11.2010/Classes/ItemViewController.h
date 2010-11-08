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

@class ItemViewControllerDelegate;

@protocol ItemViewControllerDelegate <NSObject>;

/**
 * Upon selection/taping on a list item defined by its index
 * @param iItemIndex int
 * @author Lukas N Mueller
 */
-(void) didSelectItem:(int)iItemIndex;

@end


@interface ItemViewController : UIViewController<UIScrollViewDelegate, ItemDelegate> 
{
	CGRect itemArea;
	
	id<ItemViewControllerDelegate> delegate;

}

@property(nonatomic, assign) id<ItemViewControllerDelegate> delegate;


/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)initItemView;


- (void)addTapItem;

-(void)tapOnItem:(int)iItemIndex;

@end
