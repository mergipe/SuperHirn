//
//  Item.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>


@class ItemDelegate;

@protocol ItemDelegate <NSObject>;

/**
 * Upon selection/taping on an item defined by its index
 * @param iItemIndex int
 * @author Lukas N Mueller
 */
-(void)tapOnItem:(int)iItemIndex;

@end



@interface Item : UIView 
{
	
	id<ItemDelegate> delegate;
	
@protected
	
	int itemIndex;
	CGRect itemArea;	
	UIView* borderRectangle;	
}

@property(nonatomic, assign) id<ItemDelegate> delegate;

/**
 * Set the index of this item.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void)setItemIndex:(int)iIndex;


/**
 * Add a border with a given size (pixel)  and color to surround the displayed item.
 * @param iSize CGFloat
 * @param iColor UIColor*
 * @author Lukas Mueller
 */
- (void) addBorder:(CGFloat)iSize :(UIColor*)iColor;

@end
