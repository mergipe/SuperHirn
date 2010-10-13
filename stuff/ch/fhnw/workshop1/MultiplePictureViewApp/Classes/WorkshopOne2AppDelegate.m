//
//  WorkshopOne2AppDelegate.m
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import "WorkshopOne2AppDelegate.h"


#import "ItemViewController.h"
#import "SwipeViewController.h"
#import "FullScreenViewController.h"
#import "ListViewController.h"

#import <QuartzCore/QuartzCore.h>


@implementation WorkshopOne2AppDelegate

@synthesize window;
@synthesize _swipeView;
@synthesize _fullView;
@synthesize _itemView;
@synthesize _listView;
@synthesize _navigationController;
@synthesize _tabBarController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    
	
	
	[self initControllers];
	
	[self setUpTabBarController];
	//[self setUpNavigationController];
	
	
	[window makeKeyAndVisible];
    return YES;
}

- (void) initControllers
{
	self._swipeView = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
	self._swipeView.tabBarItem.title = @"SWIPE";
	
	self._itemView = [[ItemViewController alloc] initWithNibName:@"ItemViewController" bundle:nil];
	self._itemView.tabBarItem.title = @"ITEM";

	self._listView = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];
	self._listView.tabBarItem.title = @"LIST";

	self._fullView = [[FullScreenViewController alloc] initWithNibName:@"FullScreenViewController" bundle:nil];
	self._fullView.tabBarItem.title = @"FULL";

	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPictureSwipeView:) 
												 name:@"showPictureSwipeView" object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showFullScreenPictureView:) 
												 name:@"showFullScreenPictureView" object:nil];
}	

- (void) setUpTabBarController
{
	_tabBarController = [[UITabBarController alloc] init];
	_tabBarController.viewControllers = [NSArray arrayWithObjects: _swipeView, _itemView, _fullView, _listView, nil];
	[window addSubview: [_tabBarController view]];
}

- (void) setUpNavigationController
{
	_navigationController = [[UINavigationController alloc] initWithRootViewController:_listView];
	_navigationController.navigationBar.tintColor = [UIColor blackColor];
	[window addSubview: [_navigationController view]];
}


- (void) showFullScreenPictureView:(NSNotification *)notification
{
	NSDictionary *dict = [notification userInfo];
	int index = [[dict valueForKey:@"imageIndex"] intValue];
	[_fullView showPicture:index];
	
	
	if( _tabBarController != nil )
	{
		_tabBarController.selectedIndex = 2;
	}
	else 
	{
		[ _navigationController pushViewController:_fullView animated:YES ];
	}
	
}

- (void) showPictureSwipeView:(NSNotification *)notification
{
	if( _tabBarController != nil )
	{
		_tabBarController.selectedIndex = 0;
	}
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application 
{
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc 
{
	[_fullView release];
	[_swipeView release];
    [window release];
    [super dealloc];
}


@end

