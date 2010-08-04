//
//  ArticleNavigationViewController.h
//  AkiliPad
//
//  Created by Mithin on 24/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ContentWebView;

@interface ArticleNavigationViewController : UIViewController<UIScrollViewDelegate, UIWebViewDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	NSMutableArray *articlePathList;
	
	ContentWebView *currentCover;
	ContentWebView *nextCover;
	
	NSString *articleCoverPath;
}

@property(nonatomic, retain) NSString *articleCoverPath;

- (id)initWithMagazinePath:(NSString *)magPath;

@end
