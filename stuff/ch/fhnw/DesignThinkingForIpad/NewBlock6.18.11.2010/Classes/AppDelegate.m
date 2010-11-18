//
//  AppDelegate.m
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "AppDelegate.h"

#import <QuartzCore/QuartzCore.h>


@implementation AppDelegate

@synthesize window;
@synthesize _navigationController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    	
	_navigationController = [[MainController alloc] initWithRootViewController:nil];
	[window addSubview: _navigationController.view];
	[window makeKeyAndVisible];
    return YES;
}



- (void)dealloc 
{
	[_navigationController release];
	[window release];
    [super dealloc];
}



@end

