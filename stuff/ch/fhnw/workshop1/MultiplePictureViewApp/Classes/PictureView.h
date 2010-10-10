
#import <UIKit/UIKit.h>


@interface PictureView : UIView 
{
@private
	
	BOOL fullScreenMode;
	int pictureIndex;
	CGRect imageArea;
	float scaleFactor;
	
	UIImageView *picture;
	UILabel *title;
	UIImageView *background;
		
}
@end


@interface PictureView()


/*
 * Set the title of the picture.
 * @param iTitle NSString*
 * @author Lukas Mueller
 */
- (void) setTitle:(NSString*) iTitle;

/*
 * Set full screen mode or not of picture.
 * @param iFullScreen BOOL
 * @author Lukas Mueller
 */
- (void) setFullScreen:(BOOL) iFullScreen;

/*
 * Set the index of this picture.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void)setPictureIndex:(int)iIndex;

/*
 * Set the image.
 * @param iImage NSString*
 * @author Lukas Mueller
 */
- (void) setImage:(NSString*) iImage;



@end
