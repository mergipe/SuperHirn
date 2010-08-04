//
//  LibraryViewController.h
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LibraryPageViewController;

@class LibraryModel;

@interface LibraryViewController : UIViewController<UIScrollViewDelegate> {
	IBOutlet UIScrollView *mScrollView;
	IBOutlet UIPageControl *mPageControl;
	
	LibraryPageViewController *currentPage;
	LibraryPageViewController *nextPage;
	
	LibraryModel *libModel;
}

@end
