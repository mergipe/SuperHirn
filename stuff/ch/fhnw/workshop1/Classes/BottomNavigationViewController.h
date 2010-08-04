//
//  AkiliPad
//
//  Created by Mithin on 04/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "BottomNavigationItemView.h"

@interface BottomNavigationViewController : UIViewController<BottomNavItemDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	NSMutableArray *navItems;
}

@end
