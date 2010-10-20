
#import <UIKit/UIKit.h>

#import "Item.h"

@class UIWebView;
@interface ItemWebView : Item 
{
	
@private
	
	UIWebView *webItem;
	
}
@end


@interface ItemWebView()


/*
 * Set the url of the webview.
 * @param iUrl NSString*
 * @author Lukas Mueller
 */
- (void) setUrl:(NSString*) iUrl;



@end
