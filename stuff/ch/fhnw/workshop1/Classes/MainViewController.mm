//
//  MainViewController.m
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import "MainViewController.h"
#import "RecordNavigationViewController.h"
#import "RecordViewController.h"
#import "OverviewViewController.h"

#import "MainPadAppDelegate.h"

@interface MainViewController()

- (void)showMyOverview;
- (void)loadRecordModelFrom:(NSString *)path;
- (void)loadMagazineAtPath:(NSNotification *)notification;
- (void)RecordModelLoaded;

- (void)showRecordView;
- (void)showFirstNavPointAsCoverPage;

@end

@implementation MainViewController


 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
		[appDelegate copySampleRecordToDocuments];
		viewPushed = NO;
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
		
	// set navigation label and libarary buttom
	//UILabel * label = [ iCompetenceEnvironment e5Label];
	//label.text = @"Aktuelle Ausgabe";
	//self.navigationItem.titleView = label;
	
		//Setup My Overview button
	UIBarButtonItem *libItem = [[UIBarButtonItem alloc] initWithTitle:@"Ausgaben - Das Magazin" style:UIBarButtonItemStyleBordered 
															   target:self action:@selector(showMyOverview)];
	self.navigationItem.leftBarButtonItem = libItem;
	[libItem release];
	
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
	
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	//Read the ncx and opf files
	[NSThread detachNewThreadSelector:@selector(loadRecordModelFrom:) toTarget:self withObject:basePath];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	mWindow.controllerThatObserves = self;
	mWindow.viewToObserve = mWebView;
}

- (void)viewDidAppear:(BOOL)animated {
	if(viewPushed)
		[self RecordModelLoaded];
	viewPushed = NO;
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	//mWindow.controllerThatObserves = nil;
	//mWindow.viewToObserve = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
	//UIDeviceOrientation o = [[UIDevice currentDevice] orientation];
	//if(o == UIDeviceOrientationUnknown || o == UIDeviceOrientationFaceUp || o == UIDeviceOrientationFaceDown)
	//	return NO;
	
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	
	CGRect f = mWebView.frame;
	f.size.width = self.view.frame.size.width;
	f.size.height = self.view.frame.size.height;
	mWebView.frame = f;
	
	[self RecordModelLoaded];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc {
    [super dealloc];
}


- (NSString *)coverFileName {
	/*
	NavigationModel *navModel = [NavigationModel sharedModel];
	UIInterfaceOrientation cOrientation = [self interfaceOrientation];
	if(cOrientation == UIInterfaceOrientationLandscapeLeft || cOrientation == UIInterfaceOrientationLandscapeRight ) {
		return [navModel fallbackFileName];
	}
	*/
	return @"";
}

- (void)RecordModelLoaded {
	NSLog(@"Inside RecordModelLoaded");
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	NSString *relativePath = [self coverFileName];
	if(!relativePath) {
		//The cover page is the 1st nav point.
		[self showFirstNavPointAsCoverPage];
		//if(!viewPushed) {
		//	[self showRecordView];
		//}
		return;
	}
	
	//load the cover of the magazine
	NSString *urlStr = [basePath stringByAppendingPathComponent:relativePath];
	NSURL *url = [NSURL fileURLWithPath:urlStr];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[mWebView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
	//Remove the loading view
	[mLoadingView removeFromSuperview];
}

- (void)loadRecordModelFrom:(NSString *)path {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSLog(@"Inside loadRecordModelFrom:%@", path);
	[self performSelectorOnMainThread:@selector(RecordModelLoaded) withObject:nil waitUntilDone:NO];
	
	[pool release];
}

- (void)showRecordNavigationView {
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	RecordNavigationViewController *vc = [[RecordNavigationViewController alloc] initWithMagazinePath:appDelegate.latestIssuePath];
	
	[self.navigationController pushViewController:vc animated:YES];
	[vc release];
}

- (void)showFirstNavPointAsCoverPage {
	
	NSURL *url = [NSURL fileURLWithPath:@""];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[mWebView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
	//Remove the loading view
	[mLoadingView removeFromSuperview];
}

- (void)showRecordView {
	//Show magazine view
	RecordViewController *mViewController;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		mViewController = [[RecordViewController alloc] initWithNibName:@"RecordViewController_iPad" bundle:nil];
	}
	else {
		mViewController = [[RecordViewController alloc] initWithNibName:@"RecordViewController" bundle:nil];
	}
		
	//We also have to set the page count of this Record
	mViewController.pageCount = 1;//[navModel pageCountForRecordWithPath:pathStr];
	
	[self.navigationController pushViewController:mViewController animated:YES];
	[mViewController release];
	viewPushed = YES;
}

- (void)showMyOverview {
	OverviewViewController *libVC = [[OverviewViewController alloc] initWithNibName:@"OverviewView_iPad" bundle:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadMagazineAtPath:) 
												 name:@"LoadMagazineAtPath" object:nil];
	 
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:libVC];
	[self.navigationController presentModalViewController:navigationController animated:YES];
	[navigationController autorelease];
}

- (void)loadMagazineAtPath:(NSNotification *)notification {
	NSDictionary *dict = [notification userInfo];
	NSString *path = [dict valueForKey:@"MagazinePath"];
	
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	appDelegate.latestIssuePath = path;
	//Now load this magazine.
	[NSThread detachNewThreadSelector:@selector(loadRecordModelFrom:) toTarget:self withObject:path];
	//Show loading view
	[self.view addSubview:mLoadingView];
	//Remove the notification observer
	[[NSNotificationCenter defaultCenter] removeObserver:self name:@"LoadMagazineAtPath" object:nil];
}

#pragma mark UIWebView delegate methods
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request 
											navigationType:(UIWebViewNavigationType)navigationType {
	NSString *urlString = [[request URL] absoluteString];
	NSString *fileName = [urlString lastPathComponent];
	//NSLog(@"Link Clicked = %@ ", urlString);
	if([self coverFileName] == nil) {
		return YES;
	}
	else if([fileName isEqualToString:[self coverFileName]]) {
		return YES;
	}
	
	return NO;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
	self.navigationController.navigationBar.userInteractionEnabled = YES;
}

#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point {
	//An UIWebView will always have class structure as
	//UIBrowserView->UIScrollView->UIWebView
	if(![[[view superview] superview] isKindOfClass:[UIWebView class]])
		return;
	
	//First, figure out if we have to show Record cover pages or directly the Record
	MainPadAppDelegate *delegate = [UIApplication sharedApplication].delegate;
	if([delegate displayRecordCovers]) {
		//User has clicked on the magazine, show Record lists
		[self showRecordNavigationView];
	}
	else {
		[self showRecordView];
	}
}

@end
