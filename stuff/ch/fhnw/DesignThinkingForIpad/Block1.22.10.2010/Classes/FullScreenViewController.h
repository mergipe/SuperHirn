//
//  FullScreenViewController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>


@class PictureView;
@interface FullScreenViewController : UIViewController 
{
	IBOutlet PictureView *mPicture;	
	
@private
	
	// x/y position of the picture view as well as width and height
	int imagePositionX;
	int imagePositionY;
	int imageWidth;
	int imageHeight;
}
@end

@interface FullScreenViewController()

/**
 * Show a picture at a given index.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void) showPicture:(int) iIndex;


@end
