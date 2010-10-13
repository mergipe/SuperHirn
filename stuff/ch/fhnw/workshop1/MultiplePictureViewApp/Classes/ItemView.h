
#import <UIKit/UIKit.h>


@interface ItemView : UIView 
{
	
@protected
	
	BOOL fullScreenMode;
	int itemIndex;
	CGRect itemArea;	
	UILabel *title;
	NSString* fileName; 
		
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
 * Set full screen mode or not of the item.
 * @param iFullScreen BOOL
 * @author Lukas Mueller
 */
- (void) setFullScreen:(BOOL) iFullScreen;



@end
