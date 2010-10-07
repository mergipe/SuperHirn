//
//  WorkshopOne2AppDelegate.m
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import "WorkshopOne2AppDelegate.h"


#import "RootViewController.h"
#import "DetailViewController.h"
#import "SwipeViewController.h"
#import "FullScreenViewController.h"


@implementation WorkshopOne2AppDelegate

@synthesize window;
@synthesize _swipeView;
@synthesize _fullView;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    
    
    // Add the split view controller's view to the window and display.
	// [window addSubview:splitViewController.view];
	
	 //self._swipeViewController = a;
	//_NavigationController  = [[UINavigationController alloc] initWithRootViewController:_swipeViewController];

	//[self setSwipeViewController:];
	
	//[window addSubview:_NavigationController.view];


	self._swipeView = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
	[window addSubview: [self._swipeView view]];

	self._fullView = [[FullScreenViewController alloc] initWithNibName:@"FullScreenViewController" bundle:nil];
	//[window addSubview: [self._fullView view]];
	
	[window makeKeyAndVisible];
    return YES;
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

