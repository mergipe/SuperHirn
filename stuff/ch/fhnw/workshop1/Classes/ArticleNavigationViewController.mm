//
//  ArticleNavigationViewController.m
//  AkiliPad
//
//  Created by Mithin on 24/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "ArticleNavigationViewController.h"
#import "ArticleViewController.h"

#import "ContentWebView.h"

@interface ArticleNavigationViewController(PrivateMethods)

- (void)loadArticlePaths;
- (void)applyNewIndex:(NSInteger)newIndex webView:(ContentWebView *)webView;
- (void)loadArticleAtIndex:(int)index inWebView:(ContentWebView *)webView;

@end

@implementation ArticleNavigationViewController

@synthesize articleCoverPath;

/*
// The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		//Load the articles list
    }
    return self;
}
*/

- (id)initWithMagazinePath:(NSString *)magPath {
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		self = [super initWithNibName:@"ArticleNavigationViewController_iPad" bundle:nil];
	}
	else {
		self = [super initWithNibName:@"ArticleNavigationViewController" bundle:nil];
	}
	self.articleCoverPath = magPath;
	articlePathList = [[NSMutableArray alloc] init];
	[self loadArticlePaths];
	return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	int articleCount = [articlePathList count];
	if(articleCount == 0)
		articleCount = 1;
	
	mScrollView.contentSize = CGSizeMake(articleCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	currentCover = [[ContentWebView alloc] init];
	currentCover.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	currentCover.delegate = self;
	
	nextCover = [[ContentWebView alloc] init];
	nextCover.frame = CGRectMake(mScrollView.frame.size.width, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	nextCover.delegate = self;
	
	[self loadArticleAtIndex:0 inWebView:currentCover];
	[self loadArticleAtIndex:1 inWebView:nextCover];
	
	[self applyNewIndex:0 webView:currentCover];
	[self applyNewIndex:1 webView:nextCover];
	
	[mScrollView addSubview:currentCover];
	[mScrollView addSubview:nextCover];
}


/*- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
    return YES;
}*/


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
	[currentCover release];
	[nextCover release];
	[super dealloc];
}

/*
- (void)buildArticlePathListWithNavPoint:(NCXNavPoint *)navPoint {

	
	//Only if a navPoint has label, icon or thumbnail associated with it
	//then add it to our list
	if(navPoint->title().length() || navPoint->subTitle().length() || 
	   navPoint->iconID().length() || navPoint->thumbnailID().length()) {
		NSString *relPath = [NSString stringWithUTF8String:item->href().toUtf8().data()];
		NSString *coverFilePath = [articleCoverPath stringByAppendingPathComponent:relPath];
		//NSLog(@"Addin cover path = %@", coverFilePath);
		[articlePathList addObject:coverFilePath];
	}
	 
}
*/

- (void)loadArticlePaths {
	//NSLog(@"Entering loadArticlesPath");
	/*
	NavigationModel *navModel = [NavigationModel sharedModel];
	EPubReader *reader = [navModel epubReader];
	const NCXNavMap &navMap = reader->navMap();
	QList<NCXNavPoint *>navPoints = navMap.navPoints();
	QListIterator<NCXNavPoint *> i(navPoints);
	while(i.hasNext()) {
		NCXNavPoint *navPoint = i.next();
		[self buildArticlePathListWithNavPoint:navPoint];
	}
	 */
}

- (void)applyNewIndex:(NSInteger)newIndex webView:(ContentWebView *)webView {
    NSInteger pageCount = [articlePathList count];
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

- (void)loadArticleAtIndex:(int)index inWebView:(ContentWebView *)webView {
	if(index >= [articlePathList count])
		return;
				 
	NSString *filePath = [articlePathList objectAtIndex:index];
	NSURL *url = [NSURL fileURLWithPath:filePath];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
}

- (void)showNextArticleCover:(NSNotification *)notification {
	//Remove the notification observer first
	[[NSNotificationCenter defaultCenter] removeObserver:self name:@"ShowNextArticleCover" object:nil];
	//Now, go to next cover
	CGPoint offset = mScrollView.contentOffset;
	offset.x += mScrollView.frame.size.width;
	[mScrollView setContentOffset:offset];
	//Pop out the content view
	[self.navigationController popViewControllerAnimated:YES];
}

#pragma mark UIScrollView delegate methods
- (void)scrollViewDidScroll:(UIScrollView *)sender {
	CGFloat pageWidth = mScrollView.frame.size.width;
	float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    if(fractionalPage < 0)
		return;
	
	NSInteger lowerNumber = floor(fractionalPage);
	NSInteger upperNumber = lowerNumber + 1;
    
	if (lowerNumber == currentCover.pageIndex) {//User scrolling forward
		if (upperNumber != nextCover.pageIndex && upperNumber < [articlePathList count]) {
			[self applyNewIndex:upperNumber webView:nextCover];
			[self loadArticleAtIndex:upperNumber inWebView:nextCover];
		}
	}
	else if (upperNumber == currentCover.pageIndex) {//User scrolling backward
		if (lowerNumber != nextCover.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber webView:nextCover];
			[self loadArticleAtIndex:lowerNumber inWebView:nextCover];
		}
	}
	else {
		if(lowerNumber == nextCover.pageIndex) {
			[self applyNewIndex:upperNumber webView:currentCover];
		}
		else if(upperNumber == nextCover.pageIndex) {
			[self applyNewIndex:lowerNumber webView:currentCover];
		}
		else {
			[self applyNewIndex:lowerNumber webView:currentCover];
			[self applyNewIndex:upperNumber webView:nextCover];
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView {
	CGFloat pageWidth = mScrollView.frame.size.width;
    float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    NSInteger nearestNumber = lround(fractionalPage);
	
    if (currentCover.pageIndex != nearestNumber) {
        ContentWebView *swapCover = currentCover;
        currentCover = nextCover;
        nextCover = swapCover;
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView {
	[self scrollViewDidEndScrollingAnimation:mScrollView];
}

#pragma mark UIWebView delegate methods
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request 
											navigationType:(UIWebViewNavigationType)navigationType {
	NSString *pathStr = [[request URL] path];
	//NSLog(@"Path str = %@", pathStr);
	if(webView == currentCover) {
		NSString *pStr = [articlePathList objectAtIndex:currentCover.pageIndex];
		if([pStr isEqualToString:pathStr]) {
			return YES;
		}
	}
	else if(webView == nextCover) {
		NSString *pStr = [articlePathList objectAtIndex:nextCover.pageIndex];
		if([pStr isEqualToString:pathStr]) {
			return YES;
		}
	}

	//Show magazine view
	ArticleViewController *mViewController;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		mViewController = [[ArticleViewController alloc] initWithNibName:@"ArticleViewController_iPad" bundle:nil];
	}
	else {
		mViewController = [[ArticleViewController alloc] initWithNibName:@"ArticleViewController" bundle:nil];
	}
	//Setup notification
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showNextArticleCover:) 
												 name:@"ShowNextArticleCover" object:mViewController];
	
	//NavigationModel *navModel = [NavigationModel sharedModel];
	//First, Set the current spine index in the model
	//[navModel setSpineIndexForArticleFilePath:pathStr];
	//We also have to set the page count of this article
	//mViewController.pageCount = [navModel pageCountForArticleWithPath:pathStr];
	
	[self.navigationController pushViewController:mViewController animated:YES];
	[mViewController release];
	

	return NO;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
	self.navigationController.navigationBar.userInteractionEnabled = YES;
}

@end
