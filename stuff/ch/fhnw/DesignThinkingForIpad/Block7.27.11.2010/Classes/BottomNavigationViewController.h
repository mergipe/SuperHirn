//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "BottomNavigationItemView.h"

@class BottomNavigationViewController;

@protocol BottomNavigationDelegate <NSObject>
- (void)tappedOnBottomNavigation:(BottomNavigationViewController *)viewController onItemAtIndex:(int)index;
@end


@interface BottomNavigationViewController : UIViewController<BottomNavItemDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	int ItemSpacing;
	int ScrollViewHeight;
	CGFloat fScrollWidth;	
	NSMutableArray *navItems;

	id<BottomNavigationDelegate> delegate;
}

@property(nonatomic, assign) id<BottomNavigationDelegate> delegate;

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
