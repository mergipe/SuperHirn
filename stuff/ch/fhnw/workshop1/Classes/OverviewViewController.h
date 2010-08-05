//
//  OverviewViewController.h
//  iCompetence
//
//  Authors: SH/LM on 07/06/10.
//  
//

#import <UIKit/UIKit.h>

@class OverviewPageViewController;

@class RecordModel;

@interface OverviewViewController : UIViewController<UIScrollViewDelegate> {
	IBOutlet UIScrollView *mScrollView;
	IBOutlet UIPageControl *mPageControl;
	
	OverviewPageViewController *currentPage;
	OverviewPageViewController *nextPage;
	
	RecordModel *libModel;
}

@end
