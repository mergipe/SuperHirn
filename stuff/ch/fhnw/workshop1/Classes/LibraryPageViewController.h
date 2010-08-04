//
//  LibraryPageViewController.h
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "LibraryItemView.h"

@interface LibraryPageViewController : UIViewController<LibraryItemDelegate> {
	int pageIndex;
	
	NSMutableArray *libraryItems;
}

@property(nonatomic) int pageIndex;

- (void)loadLibraryForPageAtIndex:(int)index;

@end
