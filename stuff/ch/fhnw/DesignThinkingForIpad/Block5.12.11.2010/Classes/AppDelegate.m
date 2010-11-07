//
//  AppDelegate.m
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "AppDelegate.h"

#import "SwipeViewController.h"
#import "ItemViewController.h"


#import <QuartzCore/QuartzCore.h>


@implementation AppDelegate


@synthesize window;
@synthesize _swipeView;
@synthesize _itemView;
@synthesize _navigationController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    
	[self initControllers];
	[window makeKeyAndVisible];
    return YES;
}


- (void) initControllers
{
	
	// initialization of the pointers to the controllers
	self._swipeView = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
	self._itemView = [[ItemViewController alloc] initWithNibName:@"ItemViewController" bundle:nil];
	//_itemView.delegate = self;
	
	// initialization of the pointers to the main navigation controller
	_navigationController = [[UINavigationController alloc] initWithRootViewController:_itemView];
	_navigationController.navigationBar.tintColor = [UIColor blackColor];
	[window addSubview: [_navigationController view]];
	
}	



- (void) showSwipeView:(int) iItemIndexToShow
{
	NSLog(@"AppDelegate: Show swipe view at item %d.", iItemIndexToShow);

	// first, center the swipe view to the taped item
	[ _swipeView centerAtItem: iItemIndexToShow ];
	
	// now push the swipe view in front:
	[_navigationController pushViewController:_swipeView animated:YES];
}



-(void) didSelectItem:(int)iItemIndex
{
	NSLog(@"Delegate received tap event from ListViewController: item %d", iItemIndex);
	[self showSwipeView:iItemIndex];
}


- (void)dealloc 
{
	[_navigationController release];
	[_itemView release];
	[_swipeView release];
    [window release];
    [super dealloc];
}



@end

