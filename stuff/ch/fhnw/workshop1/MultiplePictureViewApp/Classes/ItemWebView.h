
#import <UIKit/UIKit.h>

#import "ItemView.h"

@class UIWebView;
@interface ItemWebView : ItemView 
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
