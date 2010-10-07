
#import <UIKit/UIKit.h>


@protocol PictureViewDelegate <NSObject>

@optional
- (void)tappedOnPicture:(UIView *)view;

@end


@interface PictureView : UIView 
{
	
	
	
@private
	
	CGRect imageArea;
	float scaleFactor;
	
	UIImageView *picture;
	UILabel *title;
	UIImageView *background;
		
	id <PictureViewDelegate> delegate;
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
 * Set the image.
 * @param iImage UIImageView*
 * @author Lukas Mueller
 */
- (void) setImage:(UIImage*) iImage;



@end
