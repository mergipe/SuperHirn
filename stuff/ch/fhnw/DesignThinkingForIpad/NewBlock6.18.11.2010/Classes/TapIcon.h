//
//  TapIcon.h
//  iPad Block 1-6 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#include "Item.h"

#import <UIKit/UIKit.h>


@interface TapIcon : Item 
{
}


/**
 * Set the text of the tap item
 * @param iTitle NSString*
 * @author Lukas N Mueller
 */
-(void)setTitle:(NSString*)iTitle;


/**
 * Set an icon image of the tap item
 * @param iImage UIImage*
 * @author Lukas N Mueller
 */
-(void)setIcon:(UIImage*)iImage;

@end
