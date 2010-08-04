//
//  ImageBrowserViewController.h
//  AkiliPad
//

#import <UIKit/UIKit.h>

#import "Window.h"

@class ImageView;

@interface ImageBrowserViewController : UIViewController<UIScrollViewDelegate, WindowDelegate> {
	IBOutlet UIScrollView *mScrollView;
	UILabel *titleLabel;
	
	Window *mWindow;
	
	ImageView *m_pImageView;
	
	
@private
		/// x/y position of the legend view
	CGPoint legendPosition;
}


@end
