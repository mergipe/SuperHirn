//
//  MainViewController.h
//  AkiliPad
//
//  Created by Mithin on 24/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Window.h"

@interface MainViewController : UIViewController<UIWebViewDelegate, WindowDelegate> {
	IBOutlet UIWebView *mWebView;
	IBOutlet UIView *mLoadingView;
	
	Window *mWindow;
	
	BOOL viewPushed;
}

@end
