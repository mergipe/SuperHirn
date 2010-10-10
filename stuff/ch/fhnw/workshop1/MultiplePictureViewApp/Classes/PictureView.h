
#import <UIKit/UIKit.h>


@protocol PictureViewDelegate <NSObject>

@optional
- (void)tappedOnPicture:(int)onPictureIndex;

@end


@interface PictureView : UIView 
{
	id <PictureViewDelegate> delegate;
	
	
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

@property(nonatomic, assign) id<PictureViewDelegate> delegate;


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
