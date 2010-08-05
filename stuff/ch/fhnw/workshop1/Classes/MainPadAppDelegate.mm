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

- (void)copySampleRecordToDocuments {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	// Get the path to the documents directory
	NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [documentPaths objectAtIndex:0];
	//Set the books path
	NSString *RecordPathInDoc = [documentsDir stringByAppendingPathComponent:@"Record"];
	//NSLog(@"Record Path In Documents = %@", RecordPathInDoc);
	BOOL isDir;
	if ([fileManager fileExistsAtPath:RecordPathInDoc isDirectory:&isDir] && isDir) {
		//Record already eixists.
		//Check if latest path exists or not
		if(self.latestIssuePath == nil || [self.latestIssuePath length] == 0) {
			NSArray *arr = [fileManager directoryRecordsAtPath:RecordPathInDoc];
			self.latestIssuePath = [RecordPathInDoc stringByAppendingPathComponent:[arr objectAtIndex:0]];
		}
		return;
	}
	
	NSError *error = nil;
	NSString *RecordPathFromApp = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Record"];
	
	if([fileManager fileExistsAtPath:RecordPathFromApp isDirectory:&isDir] && isDir) {
		NSLog(@"Record Path exists in app");
	}
	
	if (![fileManager copyItemAtPath:RecordPathFromApp toPath:RecordPathInDoc error:&error]) {
		NSLog(@"Error while moving Record = ", [error localizedDescription]);
	}
	//The sample Record is copied. We now have to save the latest cover page path
	NSArray *arr = [fileManager directoryRecordsAtPath:RecordPathInDoc];
	self.latestIssuePath = [RecordPathInDoc stringByAppendingPathComponent:[arr objectAtIndex:0]];
	NSLog(@"Latest Issue Path = %@", latestIssuePath);
	[pool release];
}

- (NSString *)RecordBasePath {
	NSArray *documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [documentPaths objectAtIndex:0];
	NSString *fPath;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		fPath = [documentsDir stringByAppendingPathComponent:@"Record"];
	}
	else {
		fPath = [documentsDir stringByAppendingPathComponent:@"Record_iPhone"];
	}
	return fPath;
}

- (BOOL)displayRecordCovers {
	return [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"DISPLAY_Record_COVERS"] boolValue];
}

@end
