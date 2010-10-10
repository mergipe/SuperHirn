
#import <UIKit/UIKit.h>


@interface ItemView : UIView 
{
	
@private
	
	int itemIndex;
	CGRect imageArea;
	
	UIImageView *picture;
	UILabel *title;
	UIImageView *background;
		
}
@end


@interface ItemView()


/*
 * Set the title of the item.
 * @param iTitle NSString*
 * @author Lukas Mueller
 */
- (void) setTitle:(NSString*) iTitle;


/*
 * Set the index of this item.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void)setItemIndex:(int)iIndex;

/*
 * Set the image.
 * @param iImage NSString*
 * @author Lukas Mueller
 */
- (void) setImage:(NSString*) iImage;



@end
