//
//  MainViewController.h
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import <UIKit/UIKit.h>

#import "Window.h"

@interface MainViewController : UIViewController<UIWebViewDelegate, WindowDelegate> 
{
	IBOutlet UIWebView *mWebView;
	//IBOutlet UIView *mLoadingView;

	Window *mWindow;
}

@end
