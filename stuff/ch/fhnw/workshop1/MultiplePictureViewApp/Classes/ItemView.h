
#import <UIKit/UIKit.h>


@protocol ItemViewDelegate <NSObject>

@optional
- (void)tappedOnItem:(int)itemIndex;

@end


@interface ItemView : UIView 
{
	id <ItemViewDelegate> delegate;
	
	
@private
	
	int itemIndex;
	CGRect imageArea;
	
	UIImageView *picture;
	UILabel *title;
	UIImageView *background;
		
}
@end


@interface ItemView()

@property(nonatomic, assign) id<ItemViewDelegate> delegate;


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
