//
//  MagazineViewController.m
//  AkiliPad
//
//  Created by Mithin on 21/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "ArticleViewController.h"
#import "ArticleSelectorViewController.h"
#import "ImageBrowserViewController.h"

#import "AkiliPadAppDelegate.h"
#import "ArticleNavigationLeftItem.h"
#import "ContentWebView.h"
#import "ImageHandler.h"

#include <iostream>

using namespace std;

@interface ArticleViewController(PrivateMethods)

- (void)initPageViews;
- (void)loadArticlesInView;
- (void)loadContentPaths;
- (void)loadHorizontalContentPaths;

- (void)applyNewIndex:(NSInteger)newIndex webView:(ContentWebView *)webView;
- (void)loadContentAtIndex:(int)index inWebView:(ContentWebView *)webView;

- (void)loadArticleWithBottomNavIndex:(NSNotification *)notification;
- (void)toggleFullScreenMode;

@end


@implementation ArticleViewController

@synthesize articleSelector;
@synthesize articleSelectorPopover;
@synthesize pageCount;


 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		// Custom initialization
		mImageHandler = [[ImageHandler alloc] init];
		/*
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadArticleWithBottomNavIndex:) 
													name:@"LoadArticleWithBottomNavIndex" object:mBottomNav];
	
		 */
		verticalContentPathList = [[NSMutableArray alloc] init];
		horizontalContentPathList = [[NSMutableArray alloc] init];
		currentArticleIndex = 0;
		viewPushed = NO;
    }
    return self;
}

- (void)dealloc {
	[mImageHandler release];
	[currentPage release];
	[nextPage release];
	[articleSelector release];
	[articleSelectorPopover release];
	[verticalContentPathList release];
	[horizontalContentPathList release];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:@"LoadArticleWithBottomNavIndex" object:nil];
	
	[super dealloc];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	//For iPad we need to customize the title bar
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		ArticleNavigationLeftItem *leftItemView = [[ArticleNavigationLeftItem alloc] initForViewController:self];
		UIBarButtonItem *leftItem = [[UIBarButtonItem alloc] initWithCustomView:leftItemView];
		self.navigationItem.leftBarButtonItem = leftItem;
		[leftItem release];
	}
	else {
		
	}
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
	
	self.navigationController.navigationBar.hidden = YES;
	
	[self loadContentPaths];
	
	if([self interfaceOrientationLandscape]) {
		NSArray *subArray;
		pageCount = 0;
		for(subArray in horizontalContentPathList) {
			pageCount += [subArray count];
		}
	}
	else {
		pageCount = [verticalContentPathList count];
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
		//We have to load from horizontal content list
		NSArray *subArray;
		for(subArray in horizontalContentPathList) {
			pageCount += [subArray count];
		}
	}
	else {
		pageCount = [verticalContentPathList count];
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
	
	[self loadArticlesInView];
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

- (void)loadArticlesInView {
	if([self interfaceOrientationLandscape]) {
		int pCount = 0;
		for(int i = 0; i < currentArticleIndex; i++) {
			pCount += [[horizontalContentPathList objectAtIndex:i] count];
		}
		int currentPageIndex = pCount;
		int nextPageIndex = currentPageIndex+1;
		
		[self loadContentAtIndex:currentPageIndex inWebView:currentPage];
		[self loadContentAtIndex:nextPageIndex inWebView:nextPage];
		[self applyNewIndex:currentPageIndex webView:currentPage];
		[self applyNewIndex:nextPageIndex webView:nextPage];
		
		mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
		[mScrollView setContentOffset:CGPointMake(currentPageIndex * mScrollView.frame.size.width, 0.0) animated:NO];
		
	}
	else {
		int nextArticleIndex = currentArticleIndex + 1;
	
		[self loadContentAtIndex:currentArticleIndex inWebView:currentPage];
		[self loadContentAtIndex:nextArticleIndex inWebView:nextPage];
		[self applyNewIndex:currentArticleIndex webView:currentPage];
		[self applyNewIndex:nextArticleIndex webView:nextPage];
		
		mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
		[mScrollView setContentOffset:CGPointMake(currentArticleIndex * mScrollView.frame.size.width, 0.0) animated:NO];
	}
}

- (void)initPageViews {
	mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	if(currentPage == nil) {
		currentPage = [[ContentWebView alloc] init];
		currentPage.delegate = self;
		currentPage.backgroundColor = [UIColor clearColor];
	}
	currentPage.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	if(nextPage == nil) {
		nextPage = [[ContentWebView alloc] init];
		nextPage.delegate = self;
		nextPage.backgroundColor = [UIColor clearColor];
	}
	nextPage.frame = CGRectMake(mScrollView.frame.size.width, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	[self loadContentAtIndex:0 inWebView:currentPage];
	[self loadContentAtIndex:1 inWebView:nextPage];
	
	[self applyNewIndex:0 webView:currentPage];
	[self applyNewIndex:1 webView:nextPage];
	
	if([currentPage superview] == nil) {
		[mScrollView addSubview:currentPage];
	}
	if([nextPage superview] == nil) {
		[mScrollView addSubview:nextPage];
	}
}


- (void)loadVerticalContentPaths {
	//Clear the list first
	[verticalContentPathList removeAllObjects];
	
}

- (void)loadHorizontalContentPaths {
	//Clear the list first
	[horizontalContentPathList removeAllObjects];
}

- (void)loadContentPaths {
	[self loadVerticalContentPaths];
	[self loadHorizontalContentPaths];
}

- (void)applyNewIndex:(NSInteger)newIndex webView:(ContentWebView *)webView {
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

- (void)loadContentAtIndex:(int)index inWebView:(ContentWebView *)webView {
	if(index >= pageCount)
		return;
	
	NSString *filePath = nil;
	if([self interfaceOrientationLandscape]) {
		int idx = 0;
		BOOL found = NO;
		for(int i = 0; i < [horizontalContentPathList count]; i++) {
			NSArray *subArray = [horizontalContentPathList objectAtIndex:i];
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
		filePath = [verticalContentPathList objectAtIndex:index];
	}
	
	webView.hidden = YES;
	NSURL *url = [NSURL fileURLWithPath:filePath];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
}

- (void)goUpToNextArticleCover {
}

- (void)reloadArticle {
	mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	[self loadContentPaths];
	
	[self loadContentAtIndex:0 inWebView:currentPage];
	[self loadContentAtIndex:1 inWebView:nextPage];
	
	[self applyNewIndex:0 webView:currentPage];
	[self applyNewIndex:1 webView:nextPage];
}

- (void)loadArticleWithBottomNavIndex:(NSNotification *)notification {
	NSDictionary *dict = [notification userInfo];
	int index = [[dict valueForKey:@"BottomNavIndex"] intValue];
	[self toggleFullScreenMode];
	currentArticleIndex = index;
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
	ImageBrowserViewController	*imgVC = [[ImageBrowserViewController alloc] 
																initWithImageElementId:tappedElement 
																andImageHandler:mImageHandler];
		
	UINavigationController *navigationController = [[[UINavigationController alloc] initWithRootViewController:imgVC] autorelease];
	[self.navigationController presentModalViewController:navigationController animated:YES];
	[imgVC release];
	viewPushed = YES;
}

- (void)loadImageData {
	 if( ![self interfaceOrientationLandscape] ) {
		if(mImageHandler.contentView != currentPage) {
			mImageHandler.contentView = currentPage;
			[mImageHandler loadImageData];
		}
	}
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
	if(self.articleSelectorPopover != nil && self.articleSelectorPopover.popoverVisible) {
		[self.articleSelectorPopover dismissPopoverAnimated:YES];
		return;
	}
	
	if (articleSelector == nil) {
		self.articleSelector = [[ArticleSelectorViewController alloc] init];
		articleSelector.delegate = self;
		UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:articleSelector];	
		self.articleSelectorPopover = [[UIPopoverController alloc] initWithContentViewController:nav];
		[nav autorelease];
	}
	//Show if hidden
	UIBarButtonItem *item = self.navigationItem.leftBarButtonItem;
	ArticleNavigationLeftItem *itemView = (ArticleNavigationLeftItem *)item.customView;
	UIButton *b = (UIButton *)[itemView viewWithTag:2];
	[self.articleSelectorPopover presentPopoverFromRect:b.frame inView:itemView
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
			[self loadContentAtIndex:upperNumber inWebView:nextPage];
		}
		else if(upperNumber == pageCount) {
			
		}
	}
	else if (upperNumber == currentPage.pageIndex) {//User scrolling backward
		if (lowerNumber != nextPage.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber webView:nextPage];
			[self loadContentAtIndex:lowerNumber inWebView:nextPage];
			nextPage.hidden = YES;
		}
	}
	else {
		if(lowerNumber == nextPage.pageIndex) {
			[self applyNewIndex:upperNumber webView:currentPage];
			[self loadContentAtIndex:upperNumber inWebView:currentPage];
		}
		else if(upperNumber == nextPage.pageIndex) {
			[self applyNewIndex:lowerNumber webView:currentPage];
			[self loadContentAtIndex:lowerNumber inWebView:currentPage];
		}
		else {
			nextPage.hidden = YES;
			[self applyNewIndex:lowerNumber webView:currentPage];
			[self applyNewIndex:upperNumber webView:nextPage];
			[self loadContentAtIndex:upperNumber inWebView:nextPage];
			[self loadContentAtIndex:lowerNumber inWebView:currentPage];
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView {
	CGFloat pageWidth = mScrollView.frame.size.width;
    float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    NSInteger nearestNumber = lround(fractionalPage);
	
	if (currentPage.pageIndex != nearestNumber) {
		NSLog(@"Swapping webviews");
		ContentWebView *swapPage = currentPage;
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
		//save the current article index
		currentArticleIndex = currentPage.pageIndex;
		
		if(mImageHandler.contentView != currentPage) {
			mImageHandler.contentView = currentPage;
			[mImageHandler loadImageData];
		}
	}
	else {
		//Update the current article index
		int c = 0;
		int i = 0;
		currentArticleIndex = 0;
		NSArray *subArray;
		for(subArray in horizontalContentPathList) {
			c += [subArray count];
			i++;
			if(currentPage.pageIndex+1 > c) {
				currentArticleIndex = i;
			}
		}
	}
}

#pragma mark ArticleSelectorViewController delegate methods
- (void)articleSelectedWithID:(NSString *)articleId {
	//NavigationModel *navModel = [NavigationModel sharedModel];
	[self.articleSelectorPopover dismissPopoverAnimated:YES];
}

#pragma mark UIWebView delegate methods
- (void)webViewDidFinishLoad:(UIWebView *)webView {
	//We have to make sure we load all the image data
	//NSLog(@"Path = %@", [[webView.request URL] absoluteString]);
	if( ![self interfaceOrientationLandscape] ) {
		 if(webView == currentPage) {
			mImageHandler.contentView = webView;
			[mImageHandler loadImageData];
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
	
	ContentWebView *cView = (ContentWebView *)v;
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
