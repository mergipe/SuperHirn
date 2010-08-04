//
//  MagazineViewController.h
//  AkiliPad
//
//  Created by Mithin on 21/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ArticleSelectorViewController.h"

#import "Window.h"

@class UIPopoverController;
@class BottomNavigationViewController;
@class ContentWebView;

@class ImageHandler;

@interface ArticleViewController : UIViewController<UIWebViewDelegate, ArticleSelectorDelegate, 
															UIScrollViewDelegate, WindowDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	Window *mWindow;
																
	ArticleSelectorViewController *articleSelector;
	UIPopoverController *articleSelectorPopover;
	BottomNavigationViewController *mBottomNav;
	
	NSMutableArray *verticalContentPathList;
	NSMutableArray *horizontalContentPathList;
																
	ContentWebView *currentPage;
	ContentWebView *nextPage;

	ImageHandler *mImageHandler;
																
	int pageCount;
	int currentArticleIndex;
	BOOL viewPushed;
}


/**
 Checks if a image gallery loading is required based on the device orientation
 @author Lukas Mueller
 @returns BOOL true if gallery is requried, else false.
 */
- (BOOL)interfaceOrientationLandscape;


@property(nonatomic, retain) ArticleSelectorViewController *articleSelector;
@property(nonatomic, retain) UIPopoverController *articleSelectorPopover;

@property(nonatomic) int pageCount;

- (IBAction)goBack:(id)sender;
- (IBAction)showTOC:(id)sender;

@end
