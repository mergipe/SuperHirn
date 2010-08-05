//
//  OverviewPageViewController.h
//  iCompetence
//
//  Authors: SH/LM on 07/06/10.
//  
//

#import <UIKit/UIKit.h>

#import "OverviewItemView.h"

@interface OverviewPageViewController : UIViewController<OverviewItemDelegate> {
	int pageIndex;
	
	NSMutableArray *OverviewItems;
}

@property(nonatomic) int pageIndex;

- (void)loadOverviewForPageAtIndex:(int)index;

@end
