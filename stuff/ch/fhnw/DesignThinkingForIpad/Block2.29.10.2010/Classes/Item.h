//
//  Item.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>


@interface Item : UIView 
{
	
@protected
	
	int itemIndex;
	CGRect itemArea;	
	UIView* borderRectangle;
		
}
@end


@interface Item()


/**
 * Set the index of this item.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void)setItemIndex:(int)iIndex;

/**
 * Adds a fix border of 20 pixels with green color to surround the displayed item.
 * @author Lukas Mueller
 */
- (void) addBorder;

/**
 * Add a border with a given size (pixel)  and color to surround the displayed item.
 * @param iSize CGFloat
 * @param iColor UIColor*
 * @author Lukas Mueller
 */
- (void) addBorder:(CGFloat)iSize andColor:(UIColor*)iColor;

@end
