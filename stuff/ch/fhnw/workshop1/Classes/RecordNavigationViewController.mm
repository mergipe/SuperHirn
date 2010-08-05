//
//  RecordNavigationViewController.m
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import "RecordNavigationViewController.h"
#import "RecordViewController.h"

#import "RecordWebView.h"

@interface RecordNavigationViewController(PrivateMethods)

- (void)loadRecordPaths;
- (void)applyNewIndex:(NSInteger)newIndex webView:(RecordWebView *)webView;
- (void)loadRecordAtIndex:(int)index inWebView:(RecordWebView *)webView;

@end

@implementation RecordNavigationViewController

@synthesize RecordCoverPath;

/*
// The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		//Load the Records list
    }
    return self;
}
*/

- (id)initWithMagazinePath:(NSString *)magPath {
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		self = [super initWithNibName:@"RecordNavigationViewController_iPad" bundle:nil];
	}
	else {
		self = [super initWithNibName:@"RecordNavigationViewController" bundle:nil];
	}
	self.RecordCoverPath = magPath;
	RecordPathList = [[NSMutableArray alloc] init];
	[self loadRecordPaths];
	return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	int RecordCount = [RecordPathList count];
	if(RecordCount == 0)
		RecordCount = 1;
	
	mScrollView.contentSize = CGSizeMake(RecordCount * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	currentCover = [[RecordWebView alloc] init];
	currentCover.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	currentCover.delegate = self;
	
	nextCover = [[RecordWebView alloc] init];
	nextCover.frame = CGRectMake(mScrollView.frame.size.width, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	nextCover.delegate = self;
	
	[self loadRecordAtIndex:0 inWebView:currentCover];
	[self loadRecordAtIndex:1 inWebView:nextCover];
	
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
- (void)buildRecordPathListWithNavPoint:(NCXNavPoint *)navPoint {

	
	//Only if a navPoint has label, icon or thumbnail associated with it
	//then add it to our list
	if(navPoint->title().length() || navPoint->subTitle().length() || 
	   navPoint->iconID().length() || navPoint->thumbnailID().length()) {
		NSString *relPath = [NSString stringWithUTF8String:item->href().toUtf8().data()];
		NSString *coverFilePath = [RecordCoverPath stringByAppendingPathComponent:relPath];
		//NSLog(@"Addin cover path = %@", coverFilePath);
		[RecordPathList addObject:coverFilePath];
	}
	 
}
*/

- (void)loadRecordPaths {
	//NSLog(@"Entering loadRecordsPath");
	/*
	NavigationModel *navModel = [NavigationModel sharedModel];
	EPubReader *reader = [navModel epubReader];
	const NCXNavMap &navMap = reader->navMap();
	QList<NCXNavPoint *>navPoints = navMap.navPoints();
	QListIterator<NCXNavPoint *> i(navPoints);
	while(i.hasNext()) {
		NCXNavPoint *navPoint = i.next();
		[self buildRecordPathListWithNavPoint:navPoint];
	}
	 */
}

- (void)applyNewIndex:(NSInteger)newIndex webView:(RecordWebView *)webView {
    NSInteger pageCount = [RecordPathList count];
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
	if(index >= [RecordPathList count])
		return;
				 
	NSString *filePath = [RecordPathList objectAtIndex:index];
	NSURL *url = [NSURL fileURLWithPath:filePath];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webView loadRequest:request];
	self.navigationController.navigationBar.userInteractionEnabled = NO;
}

- (void)showNextRecordCover:(NSNotification *)notification {
	//Remove the notification observer first
	[[NSNotificationCenter defaultCenter] removeObserver:self name:@"ShowNextRecordCover" object:nil];
	//Now, go to next cover
	CGPoint offset = mScrollView.contentOffset;
	offset.x += mScrollView.frame.size.width;
	[mScrollView setRecordOffset:offset];
	//Pop out the Record view
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
		if (upperNumber != nextCover.pageIndex && upperNumber < [RecordPathList count]) {
			[self applyNewIndex:upperNumber webView:nextCover];
			[self loadRecordAtIndex:upperNumber inWebView:nextCover];
		}
	}
	else if (upperNumber == currentCover.pageIndex) {//User scrolling backward
		if (lowerNumber != nextCover.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber webView:nextCover];
			[self loadRecordAtIndex:lowerNumber inWebView:nextCover];
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
        RecordWebView *swapCover = currentCover;
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
		NSString *pStr = [RecordPathList objectAtIndex:currentCover.pageIndex];
		if([pStr isEqualToString:pathStr]) {
			return YES;
		}
	}
	else if(webView == nextCover) {
		NSString *pStr = [RecordPathList objectAtIndex:nextCover.pageIndex];
		if([pStr isEqualToString:pathStr]) {
			return YES;
		}
	}

	//Show magazine view
	RecordViewController *mViewController;
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		mViewController = [[RecordViewController alloc] initWithNibName:@"RecordViewController_iPad" bundle:nil];
	}
	else {
		mViewController = [[RecordViewController alloc] initWithNibName:@"RecordViewController" bundle:nil];
	}
	//Setup notification
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showNextRecordCover:) 
												 name:@"ShowNextRecordCover" object:mViewController];
	
	//NavigationModel *navModel = [NavigationModel sharedModel];
	//First, Set the current spine index in the model
	//[navModel setSpineIndexForRecordFilePath:pathStr];
	//We also have to set the page count of this Record
	//mViewController.pageCount = [navModel pageCountForRecordWithPath:pathStr];
	
	[self.navigationController pushViewController:mViewController animated:YES];
	[mViewController release];
	

	return NO;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
	self.navigationController.navigationBar.userInteractionEnabled = YES;
}

@end
