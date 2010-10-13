
#import <UIKit/UIKit.h>

@class UIWebView;
@interface ItemWebView : UIView 
{
	
@private
	
	int itemIndex;
	CGRect webArea;
	UIWebView *webItem;
	UILabel *title;
	
}
@end


@interface ItemWebView()


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
 * Set the url of the webview.
 * @param iUrl NSString*
 * @author Lukas Mueller
 */
- (void) setUrl:(NSString*) iUrl;



@end
