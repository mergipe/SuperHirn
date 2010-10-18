//
//  WorkshopOne2AppDelegate.m
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "WorkshopOne2AppDelegate.h"

#import "ItemViewController.h"
#import "SwipeViewController.h"
#import "FullScreenViewController.h"


#import <QuartzCore/QuartzCore.h>


@implementation WorkshopOne2AppDelegate

@synthesize window;
@synthesize _swipeView;
@synthesize _fullView;
@synthesize _itemView;
@synthesize _tabBarController;

//@synthesize _navigationController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    
	[self initControllers];
	[self setUpTabBarController];
	[window makeKeyAndVisible];
    return YES;
}


- (void) initControllers
{
	self._swipeView = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
	self._swipeView.tabBarItem.title = @"SWIPE";
	
	self._itemView = [[ItemViewController alloc] initWithNibName:@"ItemViewController" bundle:nil];
	self._itemView.tabBarItem.title = @"ITEM";

	self._fullView = [[FullScreenViewController alloc] initWithNibName:@"FullScreenViewController" bundle:nil];
	self._fullView.tabBarItem.title = @"FULL";

	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showSwipeView:) 
												 name:@"showSwipeView" object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showItemView:) 
												 name:@"showItemView" object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showFullScreenItemView:) 
												 name:@"showFullScreenItemView" object:nil];
}	

- (void) setUpTabBarController
{
	_tabBarController = [[UITabBarController alloc] init];
	_tabBarController.viewControllers = [NSArray arrayWithObjects: _swipeView, _itemView, _fullView, nil];
	[window addSubview: [_tabBarController view]];
}



- (void) showFullScreenItemView:(NSNotification *)notification
{
	NSDictionary *dict = [notification userInfo];
	int index = [[dict valueForKey:@"itemIndex"] intValue];
	[_fullView showPicture:index];
	
	
	if( _tabBarController != nil )
	{
		_tabBarController.selectedIndex = 2;
	}
}

- (void) showSwipeView:(NSNotification *)notification
{
	if( _tabBarController != nil )
	{
		_tabBarController.selectedIndex = 0;
	}
}

- (void) showItemView:(NSNotification *)notification
{
	if( _tabBarController != nil )
	{
		_tabBarController.selectedIndex = 1;
	}
}





- (void)dealloc 
{
	[_fullView release];
	[_swipeView release];
    [window release];
    [super dealloc];
}



/*
 - (void) setUpNavigationController
 {
 _navigationController = [[UINavigationController alloc] initWithRootViewController:_listView];
 _navigationController.navigationBar.tintColor = [UIColor blackColor];
 [window addSubview: [_navigationController view]];
 }
 */

@end

