//
//  MainPadAppDelegate.m
//

#import "MainPadAppDelegate.h"
#import "MainViewController.h"

@implementation MainPadAppDelegate

@synthesize window;
@synthesize latestIssuePath;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	//Read the latest path
	//Save the latest issue
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
	self.latestIssuePath = [userDefaults objectForKey:@"LatestIssuePath"];
	
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		mMainViewController = [[MainViewController alloc] initWithNibName:@"MainViewController_iPad" bundle:nil];
	}
	else {
		mMainViewController = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
	}
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

- (void)applicationWillTerminate:(UIApplication *)application {
	//Save the latest issue
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
	[userDefaults setObject:self.latestIssuePath forKey:@"LatestIssuePath"];
}

- (void)copySampleContentToDocuments {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	// Get the path to the documents directory
	NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [documentPaths objectAtIndex:0];
	//Set the books path
	NSString *contentPathInDoc = [documentsDir stringByAppendingPathComponent:@"Content"];
	//NSLog(@"Content Path In Documents = %@", contentPathInDoc);
	BOOL isDir;
	if ([fileManager fileExistsAtPath:contentPathInDoc isDirectory:&isDir] && isDir) {
		//Content already eixists.
		//Check if latest path exists or not
		if(self.latestIssuePath == nil || [self.latestIssuePath length] == 0) {
			NSArray *arr = [fileManager directoryContentsAtPath:contentPathInDoc];
			self.latestIssuePath = [contentPathInDoc stringByAppendingPathComponent:[arr objectAtIndex:0]];
		}
		return;
	}
	
	NSError *error = nil;
	NSString *contentPathFromApp = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Content"];
	
	if([fileManager fileExistsAtPath:contentPathFromApp isDirectory:&isDir] && isDir) {
		NSLog(@"Content Path exists in app");
	}
	
	if (![fileManager copyItemAtPath:contentPathFromApp toPath:contentPathInDoc error:&error]) {
		NSLog(@"Error while moving content = ", [error localizedDescription]);
	}
	//The sample content is copied. We now have to save the latest cover page path
	NSArray *arr = [fileManager directoryContentsAtPath:contentPathInDoc];
	self.latestIssuePath = [contentPathInDoc stringByAppendingPathComponent:[arr objectAtIndex:0]];
	NSLog(@"Latest Issue Path = %@", latestIssuePath);
	[pool release];
}

- (NSString *)contentBasePath {
	NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [documentPaths objectAtIndex:0];
	NSString *fPath;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		fPath = [documentsDir stringByAppendingPathComponent:@"Content"];
	}
	else {
		fPath = [documentsDir stringByAppendingPathComponent:@"Content_iPhone"];
	}
	return fPath;
}

- (BOOL)displayArticleCovers {
	return [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"DISPLAY_ARTICLE_COVERS"] boolValue];
}

@end
