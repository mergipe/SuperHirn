
#import <UIKit/UIKit.h>
#import "ItemView.h"

@interface PictureView : ItemView 
{
@private
	
	float scaleFactor;	
	UIImageView *picture;

	// variables describing the border of the displayed image:
	int borderSize;
	UIColor* borderColor;
}
@end


@interface PictureView()



/*
 * Set the image path.
 * @param iImage NSString*
 * @author Lukas Mueller
 */
- (void) setImage:(NSString*) iImage;



@end
