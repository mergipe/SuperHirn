//
//  PictureView.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "Item.h"

@interface PictureView : Item 
{
@private
	
	UIImageView *picture;
}

/**
 * Set the image path.
 * @param iImage NSString*
 * @author Lukas Mueller
 */
- (void) setImage:(NSString*) iImage;

/**
 * Get the height of the image.
 * @return height
 * @author Lukas Mueller
 */
- (CGFloat) getHeight;

/**
 * Get the width of the image.
 * @return width
 * @author Lukas Mueller
 */
- (CGFloat) getWidth;


@end
