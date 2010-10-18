//
//  PictureView.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "ItemView.h"

@interface PictureView : ItemView 
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



@end
