//
//  MagazineViewController.m
//  iCompetence
//
//  Authors: SH/LM on 21/04/10.
//  
//

#import "RecordViewController.h"
#import "RecordSelectorViewController.h"
#import "ImageBrowserViewController.h"

#import "MainPadAppDelegate.h"
#import "RecordNavigationLeftItem.h"
#import "RecordWebView.h"

#include <iostream>

using namespace std;

@interface RecordViewController(PrivateMethods)

- (void)initPageViews;
- (void)loadRecordsInView;
- (void)loadRecordPaths;
- (void)loadHorizontalRecordPaths;

- (void)applyNewIndex:(NSInteger)newIndex webView:(RecordWebView *)webView;
- (void)loadRecordAtIndex:(int)index inWebView:(RecordWebView *)webView;

- (void)loadRecordWithBottomNavIndex:(NSNotification *)notification;
- (void)toggleFullScreenMode;

@end


@implementation RecordViewController

@synthesize RecordSelector;
@synthesize RecordSelectorPopover;
@synthesize pageCount;


 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		/*
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadRecordWithBottomNavIndex:) 
													name:@"LoadRecordWithBottomNavIndex" object:mBottomNav];
	
		 */
		verticalRecordPathList = [[NSMutableArray alloc] init];
		horizontalRecordPathList = [[NSMutableArray alloc] init];
		currentRecordIndex = 0;
		viewPushed = NO;
    }
    return self;
}

- (void)dealloc {
	[currentPage release];
	[nextPage release];
	[RecordSelector release];
	[RecordSelectorPopover release];
	[verticalRecordPathList release];
	[horizontalRecordPathList release];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:@"LoadRecordWithBottomNavIndex" object:nil];
	
	[super dealloc];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	//For iPad we need to customize the title bar
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		RecordNavigationLeftItem *leftItemView = [[RecordNavigationLeftItem alloc] initForViewController:self];
		UIBarButtonItem *leftItem = [[UIBarButtonItem alloc] initWithCustomView:leftItemView];
		self.navigationItem.leftBarButtonItem = leftItem;
		[leftItem release];
	}
	else {
		
	}
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
	
	self.navigationController.navigationBar.hidden = YES;
	
	[self loadRecordPaths];
	
	if([self interfaceOrientationLandscape]) {
		NSArray *subArray;
		pageCount = 0;
		for(subArray in horizontalRecordPathList) {
			pageCount += [subArray count];
		}
	}
	else {
		pageCount = [verticalRecordPathList count];
	}
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	mWindow.controllerThatObserves = self;
	mWindow.viewToObserve = self.view;
	if(!viewPushed)
		[self initPageViews];
	viewPushed = NO;
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	mWindow.controllerThatObserves = nil;
	mWindow.viewToObserve = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	self.navigationController.navigationBar.hidden = YES;

	currentPage.hidden = YES;
	nextPage.hidden = YES;
	pageCount = 0;
	if(toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || 
	   toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {
		//We have to load from horizontal Record list
		NSArray *subArray;
		for(subArray in horizontalRecordPathList) {
			pageCount += [subArray count];
		}
	}
	else {
		pageCount = [verticalRecordPathList count];
	}
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	
	self.navigationController.navigationBar.hidden = YES;
	
	//Reframe webviews
	CGRect f = currentPage.frame;
	f.size.width = self.view.frame.size.width;
	f.size.height = self.view.frame.size.height;
	currentPage.frame = f;
	
	f = nextPage.frame;
	f.size.width = self.view.frame.size.width;
	f.size.height = self.view.frame.size.height;
	nextPage.frame = f;
	
	[self loadRecordsInView];
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

- (void)loadRecordsInView {
	if([self interfaceOrientationLandscape]) {
		int pCount = 0;
		for(int i = 0; i < currentRecordIndex; i++) {
			pCount += [[horizontalRecordPathList objectAtIndex:i] count];
		}
		int currentPageIndex = pCount;
		int nextPageIndex = currentPageIndex+1;
		
		[self loadRecordAtIndex:currentPageIndex inWebView:currentPage];
		[self loadRecordAtIndex:nextPageIndex inWebView:nextPage];
		[self applyNewIndex:currentPageIndex webView:currentPage];
		[self applyNewIndex:nextPageIndex webView:nextPage];
		
		mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
		[mScrollView setRecordOffset:CGPointMake(currentPageIndex * mScrollView.frame.size.width, 0.0) animated:NO];
		
	}
	else {
		int nextRecordIndex = currentRecordIndex + 1;
	
		[self loadRecordAtIndex:currentRecordIndex inWebView:currentPage];
		[self loadRecordAtIndex:nextRecordIndex inWebView:nextPage];
		[self applyNewIndex:currentRecordIndex webView:currentPage];
		[self applyNewIndex:nextRecordIndex webView:nextPage];
		
		mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
		[mScrollView setRecordOffset:CGPointMake(currentRecordIndex * mScrollView.frame.size.width, 0.0) animated:NO];
	}
}

- (void)initPageViews {
	mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	if(currentPage == nil) {
		currentPage = [[RecordWebView alloc] init];
		currentPage.delegate = self;
		currentPage.backgroundColor = [UIColor clearColor];
	}
	currentPage.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	if(nextPage == nil) {
		nextPage = [[RecordWebView alloc] init];
		nextPage.delegate = self;
		nextPage.backgroundColor = [UIColor clearColor];
	}
	nextPage.frame = CGRectMake(mScrollView.frame.size.width, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	[self loadRecordAtIndex:0 inWebView:currentPage];
	[self loadRecordAtIndex:1 inWebView:nextPage];
	
	[self applyNewIndex:0 webView:currentPage];
	[self applyNewIndex:1 webView:nextPage];
	
	if([currentPage superview] == nil) {
		[mScrollView addSubview:currentPage];
	}
	if([nextPage superview] == nil) {
		[mScrollView addSubview:nextPage];
	}
}


- (void)loadVerticalRecordPaths {
	//Clear the list first
	[verticalRecordPathList removeAllObjects];
	
}

- (void)loadHorizontalRecordPaths {
	//Clear the list first
	[horizontalRecordPathList removeAllObjects];
}

- (void)loadRecordPaths {
	[self loadVerticalRecordPaths];
	[self loadHorizontalRecordPaths];
}

- (void)applyNewIndex:(NSInteger)newIndex webView:(RecordWebView *)webView {
    BOOL outOfBounds = newIndex >= pageCount || newIndex < 0;
	
    if (!outOfBounds) {
        CGRect pageFrame = webView.frame;
        pageFrame.origin.y = 0;
        pageFrame.origin.x = mScrollView.frame.size.width * newIndex;
        webView.frame = pageFrame;
    }
    else {
        CGRect pageFrame = webView.frame;
        pageFrame.origin.y = mScrollView.frame.size.height;
        webView.frame = pageFrame;
    }	
    webView.pageIndex = newIndex;
}

- (void)loadRecordAtIndex:(int)index inWebView:(RecordWebView *)webView {
	if(index >= pageCount)
		return;
	
	NSString *filePath = nil;
	if([self interfaceOrientationLandscape]) {
		int idx = 0;
		BOOL found = NO;
		for(int i = 0; i < [horizontalRecordPathList count]; i++) {
			NSArray *subArray = [horizontalRecordPathList objectAtIndex:i];
			for(int j = 0; j < [subArray count]; j++) {
				NSString *f = [subArray objectAtIndex:j];
				if(idx == index) {
					filePath = f;
					found = YES;
					break;
				}
				idx++;
			}
			if(found) {
				break;
			}
		}
	}
	else {
		filePath = [verticalRecordPathList objectAtIndex:index];
	}
	
	webView.hidden = YES;
	NSURL *url = [NSURL fileURLWithPath:filePath];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
}

- (void)goUpToNextRecordCover {
}

- (void)reloadRecord {
	mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	[self loadRecordPaths];
	
	[self loadRecordAtIndex:0 inWebView:currentPage];
	[self loadRecordAtIndex:1 inWebView:nextPage];
	
	[self applyNewIndex:0 webView:currentPage];
	[self applyNewIndex:1 webView:nextPage];
}

- (void)loadRecordWithBottomNavIndex:(NSNotification *)notification {
	NSDictionary *dict = [notification userInfo];
	int index = [[dict valueForKey:@"BottomNavIndex"] intValue];
	[self toggleFullScreenMode];
	currentRecordIndex = index;
}


- (void)toggleFullScreenMode {
	
	if(self.navigationController.navigationBar.hidden) {

	}
	
	/*
	CGRect f = mBottomNav.view.frame;
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.3];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	
	if(self.navigationController.navigationBar.hidden) {
		self.navigationController.navigationBar.hidden = NO;
		mBottomNav.view.center = CGPointMake(f.size.width/2, f.origin.y - f.size.height/2);
	}
	else {
		self.navigationController.navigationBar.hidden = YES;
		mBottomNav.view.center = CGPointMake(f.size.width/2, f.origin.y + (3 * f.size.height/2));
	}
	[UIView commitAnimations];
	*/
	[self performSelector:@selector(toggleNavDisplay) withObject:nil afterDelay:0.4];
}

- (void)showImageBrowserForElement:(NSString *)tappedElement {
	/*
	ImageBrowserViewController	*imgVC = [[ImageBrowserViewController alloc] 
																initWithImageElementId:tappedElement 
																andImageHandler:nil];
		
	UINavigationController *navigationController = [[[UINavigationController alloc] initWithRootViewController:imgVC] autorelease];
	[self.navigationController presentModalViewController:navigationController animated:YES];
	[imgVC release];
	viewPushed = YES;
	 */
}

- (void)loadImageData {
}

- (BOOL)interfaceOrientationLandscape {	
	// Lukas: only dont load the image gallery if the device is in any lanscape mode
	UIInterfaceOrientation cOrientation = [self interfaceOrientation];
	if(cOrientation == UIInterfaceOrientationLandscapeLeft || cOrientation == UIInterfaceOrientationLandscapeRight ) {
		return YES;
	}
	
	return NO;
}

-(void)fadeInWebView:(UIWebView *)webView {
	webView.alpha = 0.0;
	webView.hidden = NO;

	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.10];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	webView.alpha = 1.0;
	[UIView commitAnimations];
}

#pragma mark IBAction
- (IBAction)goBack:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)showTOC:(id)sender {
	//Hide if shown.
	if(self.RecordSelectorPopover != nil && self.RecordSelectorPopover.popoverVisible) {
		[self.RecordSelectorPopover dismissPopoverAnimated:YES];
		return;
	}
	
	if (RecordSelector == nil) {
		self.RecordSelector = [[RecordSelectorViewController alloc] init];
		RecordSelector.delegate = self;
		UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:RecordSelector];	
		self.RecordSelectorPopover = [[UIPopoverController alloc] initWithRecordViewController:nav];
		[nav autorelease];
	}
	//Show if hidden
	UIBarButtonItem *item = self.navigationItem.leftBarButtonItem;
	RecordNavigationLeftItem *itemView = (RecordNavigationLeftItem *)item.customView;
	UIButton *b = (UIButton *)[itemView viewWithTag:2];
	[self.RecordSelectorPopover presentPopoverFromRect:b.frame inView:itemView
							   permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

#pragma mark UIScrollView delegate methods
- (void)scrollViewDidScroll:(UIScrollView *)sender {
	CGFloat pageWidth = mScrollView.frame.size.width;
	float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    if(fractionalPage < 0)
		return;
	
	NSInteger lowerNumber = floor(fractionalPage);
	NSInteger upperNumber = lowerNumber + 1;
    
	if (lowerNumber == currentPage.pageIndex) {//User scrolling forward
		if (upperNumber != nextPage.pageIndex && upperNumber < pageCount) {
			[self applyNewIndex:upperNumber webView:nextPage];
			[self loadRecordAtIndex:upperNumber inWebView:nextPage];
		}
		else if(upperNumber == pageCount) {
			
		}
	}
	else if (upperNumber == currentPage.pageIndex) {//User scrolling backward
		if (lowerNumber != nextPage.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber webView:nextPage];
			[self loadRecordAtIndex:lowerNumber inWebView:nextPage];
			nextPage.hidden = YES;
		}
	}
	else {
		if(lowerNumber == nextPage.pageIndex) {
			[self applyNewIndex:upperNumber webView:currentPage];
			[self loadRecordAtIndex:upperNumber inWebView:currentPage];
		}
		else if(upperNumber == nextPage.pageIndex) {
			[self applyNewIndex:lowerNumber webView:currentPage];
			[self loadRecordAtIndex:lowerNumber inWebView:currentPage];
		}
		else {
			nextPage.hidden = YES;
			[self applyNewIndex:lowerNumber webView:currentPage];
			[self applyNewIndex:upperNumber webView:nextPage];
			[self loadRecordAtIndex:upperNumber inWebView:nextPage];
			[self loadRecordAtIndex:lowerNumber inWebView:currentPage];
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView {
	CGFloat pageWidth = mScrollView.frame.size.width;
    float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    NSInteger nearestNumber = lround(fractionalPage);
	
	if (currentPage.pageIndex != nearestNumber) {
		NSLog(@"Swapping webviews");
		RecordWebView *swapPage = currentPage;
		currentPage = nextPage;
		nextPage = swapPage;
	}
	else {
		nextPage.hidden = YES;
	}
	
	//[self loadImageData];
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView {
	[self scrollViewDidEndScrollingAnimation:mScrollView];
	//If we are in vertical mode then we have to load the corresponding horizontal components
	if(![self interfaceOrientationLandscape]) {
		//save the current Record index
		currentRecordIndex = currentPage.pageIndex;		
	}
	else {
		//Update the current Record index
		int c = 0;
		int i = 0;
		currentRecordIndex = 0;
		NSArray *subArray;
		for(subArray in horizontalRecordPathList) {
			c += [subArray count];
			i++;
			if(currentPage.pageIndex+1 > c) {
				currentRecordIndex = i;
			}
		}
	}
}

#pragma mark RecordSelectorViewController delegate methods
- (void)RecordSelectedWithID:(NSString *)RecordId {
	//NavigationModel *navModel = [NavigationModel sharedModel];
	[self.RecordSelectorPopover dismissPopoverAnimated:YES];
}

#pragma mark UIWebView delegate methods
- (void)webViewDidFinishLoad:(UIWebView *)webView {
	//We have to make sure we load all the image data
	//NSLog(@"Path = %@", [[webView.request URL] absoluteString]);
	if( ![self interfaceOrientationLandscape] ) {
		 if(webView == currentPage) {
		}
	}
	if(webView.hidden){
		NSLog(@"Fading out");
		[self  performSelector:@selector(fadeInWebView:) withObject:webView afterDelay:0.30];
	}
	
	self.navigationController.navigationBar.userInteractionEnabled = YES;
}

#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point {
	if([self interfaceOrientationLandscape])
		return;
	
	UIView *v = [[view superview] superview];
	if(![v isKindOfClass:[UIWebView class]])
		return;
	
	[self toggleFullScreenMode];
}

- (void)pinchedOutOnView:(UIView *)view withPoints:(NSArray *)points {
	UIDeviceOrientation cOrientation = [[UIDevice currentDevice] orientation];
	if(cOrientation == UIDeviceOrientationLandscapeRight || cOrientation == UIDeviceOrientationLandscapeLeft) {
		//Don't do anything for horizontal orientation
		return;
	}
	/*
	if(mBottomNav.view.hidden == NO) {//Do not open gallery if bottom nav is shown
		NSLog(@"Bottom view is not hidden. Returning");
		return;
	}
	 */
	
	UIView *v = [[view superview] superview];
	if(![v isKindOfClass:[UIWebView class]])
		return;
	
	RecordWebView *cView = (RecordWebView *)v;
	NSValue *pointValue;
	NSString *pinchedElement = nil;
	BOOL onImage = YES;
	for(pointValue in points) {
		CGPoint p = [pointValue CGPointValue];
		int y = [cView scrollPosition] + p.y;
		NSString *tElement = [cView elementAtX:p.x andY:y];
		if(pinchedElement == nil)
			pinchedElement = tElement;
		
		NSLog(@"Pinched in Element = %@", tElement);
		if([tElement hasPrefix:@"image"] && [pinchedElement isEqualToString:tElement]) {
			onImage = YES;
		}
		else {
			onImage = NO;
		}
		if(!onImage)
			break;
	}
	if(onImage) {
		[self showImageBrowserForElement:pinchedElement];
	}
}

@end
