//
//  ModuleInfoView.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "Item.h"

@interface ModuleInfoView : Item 
{
@private
	
	
	UILabel *text;
	UIImageView *picture;
}

/**
 * Set the image.
 * @param iImage UIImage*
 * @author Lukas Mueller
 */
- (void) setImage:(UIImage*) iImage;

/**
 * Set the text.
 * @param iText NSString*
 * @author Lukas Mueller
 */
- (void) setText:(NSString*) iText;



@end
