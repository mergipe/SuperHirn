//
//  MainPadAppDelegate.m
//

#import "MainPadAppDelegate.h"
#import "MainViewController.h"

@implementation MainPadAppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions 
{
	//Read the latest path
	//Save the latest issue
	//NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
	//self.latestIssuePath = [userDefaults objectForKey:@"LatestIssuePath"];
	

	mMainViewController = [[MainViewController alloc] initWithNibName:@"MainViewController_iPad" bundle:nil];
	mNavController = [[UINavigationController alloc] initWithRootViewController:mMainViewController];
											  
	[window addSubview:mNavController.view];
	[window makeKeyAndVisible];
	mNavController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	
	return YES;
}

- (void)dealloc {
	[mMainViewController release];
	[mNavController release];
	[latestIssuePath release];
	[super dealloc];
}

- (void)applicationWillTerminate:(UIApplication *)application 
{
}


@end
