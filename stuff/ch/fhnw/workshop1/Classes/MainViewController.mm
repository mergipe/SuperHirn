//
//  MainViewController.m
//  AkiliPad
//
//  Created by Mithin on 24/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "MainViewController.h"
#import "ArticleNavigationViewController.h"
#import "ArticleViewController.h"
#import "LibraryViewController.h"
#import "AkiliPadEnvironment.h"

#import "AkiliPadAppDelegate.h"
#import "NavigationModel.h"

#include "EPUBReader.h"
#include "OPFItem.h"

#include <QMap>
#include <QString>

@interface MainViewController()

- (void)showMyLibrary;
- (void)loadContentModelFrom:(NSString *)path;
- (void)loadMagazineAtPath:(NSNotification *)notification;
- (void)contentModelLoaded;

- (void)showArticleView;
- (void)showFirstNavPointAsCoverPage;

@end

@implementation MainViewController


 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
		[appDelegate copySampleContentToDocuments];
		viewPushed = NO;
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
		
	// set navigation label and libarary buttom
	UILabel * label = [ AkiliPadEnvironment e5Label];
	label.text = @"Aktuelle Ausgabe";
	self.navigationItem.titleView = label;
	
		//Setup My Library button
	UIBarButtonItem *libItem = [[UIBarButtonItem alloc] initWithTitle:@"Ausgaben - Das Magazin" style:UIBarButtonItemStyleBordered 
															   target:self action:@selector(showMyLibrary)];
	self.navigationItem.leftBarButtonItem = libItem;
	[libItem release];
	
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
	
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	//Read the ncx and opf files
	[NSThread detachNewThreadSelector:@selector(loadContentModelFrom:) toTarget:self withObject:basePath];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	mWindow.controllerThatObserves = self;
	mWindow.viewToObserve = mWebView;
}

- (void)viewDidAppear:(BOOL)animated {
	if(viewPushed)
		[self contentModelLoaded];
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
	
	[self contentModelLoaded];
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
	NavigationModel *navModel = [NavigationModel sharedModel];
	UIInterfaceOrientation cOrientation = [self interfaceOrientation];
	if(cOrientation == UIInterfaceOrientationLandscapeLeft || cOrientation == UIInterfaceOrientationLandscapeRight ) {
		return [navModel fallbackFileName];
	}
	
	return [navModel coverFileName];
}

- (void)contentModelLoaded {
	NSLog(@"Inside contentModelLoaded");
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	NSString *relativePath = [self coverFileName];
	if(!relativePath) {
		//The cover page is the 1st nav point.
		[self showFirstNavPointAsCoverPage];
		//if(!viewPushed) {
		//	[self showArticleView];
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

- (void)loadContentModelFrom:(NSString *)path {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSLog(@"Inside loadContentModelFrom:%@", path);
	[[NavigationModel sharedModel] loadNavigationFor:path];
	[self performSelectorOnMainThread:@selector(contentModelLoaded) withObject:nil waitUntilDone:NO];
	
	[pool release];
}

- (void)showArticleNavigationView {
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	ArticleNavigationViewController *vc = [[ArticleNavigationViewController alloc] initWithMagazinePath:appDelegate.latestIssuePath];
	
	[self.navigationController pushViewController:vc animated:YES];
	[vc release];
}

- (void)showFirstNavPointAsCoverPage {
	NavigationModel *navModel = [NavigationModel sharedModel];
	EPubReader *reader = [navModel epubReader];
	QMap<QString, OPFItem *> ocfMap = reader->manifestMap();
	const QList<QString> spines = reader->spineList();
	//NSString *articleID = [NSString stringWithUTF8String:spines[0].toUtf8().data()];
	OPFItem *item = ocfMap.value(spines[0]);
	if(item == NULL)
		return;
	
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	NSString *rootRelPath = [NSString stringWithUTF8String:reader->rootRelativeFolderPath().toUtf8().data()];
	NSString *filePath = [NSString stringWithUTF8String:item->href().toUtf8().data()];
	NSString *urlStr = [[basePath stringByAppendingPathComponent:rootRelPath] stringByAppendingPathComponent:filePath];
	NSLog(@"Loading first page with path = %@", urlStr);
	NSURL *url = [NSURL fileURLWithPath:urlStr];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[mWebView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
	//Remove the loading view
	[mLoadingView removeFromSuperview];
}

- (void)showArticleView {
	NSLog(@"Inside show Article View");
	NavigationModel *navModel = [NavigationModel sharedModel];
	EPubReader *reader = [navModel epubReader];
	const QList<QString> spines = reader->spineList();
	if(spines.count() <= 1)//there are no articles
		return;
	
	//Show magazine view
	ArticleViewController *mViewController;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		mViewController = [[ArticleViewController alloc] initWithNibName:@"ArticleViewController_iPad" bundle:nil];
	}
	else {
		mViewController = [[ArticleViewController alloc] initWithNibName:@"ArticleViewController" bundle:nil];
	}
	
	
	NSString *articleID;
	if([navModel coverFileName]) {
		articleID = [NSString stringWithUTF8String:spines[1].toUtf8().data()];
	}
	else {
		articleID = [NSString stringWithUTF8String:spines[0].toUtf8().data()];
	}
	//First, Set the current spine index in the model
	[navModel setSpineIndexForItemId:articleID];
	//We also have to set the page count of this article
	mViewController.pageCount = 1;//[navModel pageCountForArticleWithPath:pathStr];
	
	[self.navigationController pushViewController:mViewController animated:YES];
	[mViewController release];
	viewPushed = YES;
}

- (void)showMyLibrary {
	LibraryViewController *libVC = [[LibraryViewController alloc] initWithNibName:@"LibraryView_iPad" bundle:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadMagazineAtPath:) 
												 name:@"LoadMagazineAtPath" object:nil];
	 
	UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:libVC];
	[self.navigationController presentModalViewController:navigationController animated:YES];
	[navigationController autorelease];
}

- (void)loadMagazineAtPath:(NSNotification *)notification {
	NSDictionary *dict = [notification userInfo];
	NSString *path = [dict valueForKey:@"MagazinePath"];
	
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	appDelegate.latestIssuePath = path;
	//Now load this magazine.
	[NSThread detachNewThreadSelector:@selector(loadContentModelFrom:) toTarget:self withObject:path];
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
	
	//First, figure out if we have to show article cover pages or directly the article
	AkiliPadAppDelegate *delegate = [UIApplication sharedApplication].delegate;
	if([delegate displayArticleCovers]) {
		//User has clicked on the magazine, show article lists
		[self showArticleNavigationView];
	}
	else {
		[self showArticleView];
	}
}

@end
