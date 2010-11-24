//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "GalleryNavigationItemView.h"

@class GalleryNavigationViewController;

@protocol GalleryNavigationViewController <NSObject>
- (void)tappedOnBottomNavigation:(GalleryNavigationViewController *)viewController onItemAtIndex:(int)index;
@end


@interface GalleryNavigationViewController : UIViewController<GalleryNavigationItemViewDelegate > {
	IBOutlet UIScrollView *mScrollView;
	
	int ItemSpacing;
	int ScrollViewHeight;
	CGFloat fScrollWidth;	
	NSMutableArray *navItems;

	id<GalleryNavigationViewController> delegate;
}

@property(nonatomic, assign) id<GalleryNavigationViewController> delegate;

/**
 * Center the navigation bar to the icon of the article at this index
 * @param iIndex int
 * @author Lukas N Mueller
 */
- (void)centerToIndex:(int)iIndex; 


/**
 * Center the navigation bar to the icon of the article at this index
 * @param iIndex int
 * @author Lukas N Mueller
 */
- (void)addNavItem:(NSString*) iTitle;

- (void)addNavItemX:(NSString*) iTitle :(int)i;

@end
