//
//  ImageBrowserViewController.h
//  AkiliPad
//
//  Created by Mithin on 11/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Window.h"

@class AkiliImageView;

@class ImageHandler;
@class ImageLegendView;
@interface ImageBrowserViewController : UIViewController<UIScrollViewDelegate, WindowDelegate> {
	IBOutlet UIScrollView *mScrollView;
	UILabel *titleLabel;
	ImageLegendView* legend;
	
	Window *mWindow;
	
	AkiliImageView *currentImageView;
	AkiliImageView *nextImageView;
	
	ImageHandler *mImageHandler;
	int currentPageIndex;
	
@private
		/// x/y position of the legend view
	CGPoint legendPosition;
}

- (id)initWithImageElementId:(NSString *)elementId andImageHandler:(ImageHandler *)imageHandler;

@end
