//
//  ItemPageView.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>



@interface ItemPageView : UIView 
{
	
	
@private
	
	int startIndex, endIndex;	
	int iconHeight, iconWidth;
	int spacing;
}
@end


@interface ItemPageView()


+ (int) numberItemsPerPage;

/*
 * Set the title of the picture.
 * @param iTitle NSString*
 * @author Lukas Mueller
 */
- (void) setItemIndexes:(int) iStartIndex withEndIndex:(int) iEndIndex;



@end
