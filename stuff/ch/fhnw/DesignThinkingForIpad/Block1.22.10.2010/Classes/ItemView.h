//
//  ItemView.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>


@interface ItemView : UIView 
{
	
@protected
	
	BOOL fullScreenMode;
	int itemIndex;
	CGRect itemArea;	
	UILabel *title;
	NSString* fileName; 
	
	UIView* borderRectangle;
		
}
@end


@interface ItemView()


/*
 * Set the title of the item.
 * @param iTitle NSString*
 * @author Lukas Mueller
 */
- (void) setTitle:(NSString*) iTitle;


/*
 * Set the index of this item.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void)setItemIndex:(int)iIndex;

/*
 * Set full screen mode or not of the item.
 * @param iFullScreen BOOL
 * @author Lukas Mueller
 */
- (void) setFullScreen:(BOOL) iFullScreen;

/*
 * Add a border with a given size (pixel)  and color to surround the displayed item.
 * @param iSize int
 * @param iColor UIColor*
 * @author Lukas Mueller
 */
- (void) addBorder:(int)iSize andColor:(UIColor*)iColor;

@end
