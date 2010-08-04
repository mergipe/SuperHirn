    //
//  LibraryViewController.mm
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "LibraryViewController.h"
#import "LibraryPageViewController.h"
#import "LibraryModel.h"
//#import "AkiliPadEnvironment.h"

@interface LibraryViewController()

- (void)showCurrentIssue;

//- (void)loadArticlePaths;
- (void)applyNewIndex:(NSInteger)newIndex toPageController:(LibraryPageViewController *)pController;
- (void)showPageAtIndex:(int)index inController:(LibraryPageViewController *)pController;

- (void)loadMagazineAtIndex:(NSNotification *)notification;

@end


@implementation LibraryViewController

 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		libModel = [LibraryModel sharedModel];
		[libModel loadLibraryContents];
    }
    return self;
}

- (void)dealloc {
	[currentPage release];
	[nextPage release];
	
	[super dealloc];
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	//UILabel * label = [ AkiliPadEnvironment e5Label];
	//label.text = @"Ausgaben - Das Magazin";
	//self.navigationItem.titleView = label;
	self.navigationController.navigationBar.tintColor = [UIColor blackColor];
	
	UIBarButtonItem *currentIssueItem = [[UIBarButtonItem alloc] initWithTitle:@"Aktuelle Ausgabe" style:UIBarButtonItemStyleBordered 
																		target:self action:@selector(showCurrentIssue)];
	self.navigationItem.leftBarButtonItem = currentIssueItem;
	[currentIssueItem release];
	
	int pageCount = [libModel.contentList count]/9;
	if(pageCount == 0)
		pageCount = 1;
	
	mScrollView.contentSize = CGSizeMake(pageCount * mScrollView.frame.size.width, mScrollView.frame.size.height-50.0);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	currentPage = [[LibraryPageViewController alloc] initWithNibName:@"LibraryPageView_iPad" bundle:nil];
	currentPage.view.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	nextPage = [[LibraryPageViewController alloc] initWithNibName:@"LibraryPageView_iPad" bundle:nil];
	nextPage.view.frame = CGRectMake(mScrollView.frame.size.width, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loadMagazineAtIndex:) 
												 name:@"LoadMagazineAtIndex" object:nil];
	
	[self applyNewIndex:0 toPageController:currentPage];
	[self applyNewIndex:1 toPageController:nextPage];
	
	[self showPageAtIndex:0 inController:currentPage];
	[self showPageAtIndex:1 inController:nextPage];
	
	[mScrollView addSubview:currentPage.view];
	[mScrollView addSubview:nextPage.view];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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

- (void)showCurrentIssue {
	[self.navigationController dismissModalViewControllerAnimated:YES];
}

- (NSUInteger)libPageCount {
	int pageCount = [libModel.contentList count]/9;
	int m = [libModel.contentList count] % 9;
	if(m > 0) {
		pageCount++;
	}
	return pageCount;
}

- (void)applyNewIndex:(NSInteger)newIndex toPageController:(LibraryPageViewController *)pController {
	NSUInteger pageCount = [self libPageCount];
	
	BOOL outOfBounds = newIndex >= pageCount || newIndex < 0;
	
    if (!outOfBounds) {
        CGRect pageFrame = pController.view.frame;
        pageFrame.origin.y = 0;
        pageFrame.origin.x = mScrollView.frame.size.width * newIndex;
        pController.view.frame = pageFrame;
    }
    else {
        CGRect pageFrame = pController.view.frame;
        pageFrame.origin.y = mScrollView.frame.size.height;
        pController.view.frame = pageFrame;
    }	
    pController.pageIndex = newIndex;
}

- (void)showPageAtIndex:(int)index inController:(LibraryPageViewController *)pController {
	NSUInteger pageCount = [self libPageCount];
	if(index >= pageCount)
		return;
	
	[pController loadLibraryForPageAtIndex:index];
}

- (void)loadMagazineAtIndex:(NSNotification *)notification {
	NSDictionary *dict = [notification userInfo];
	int index = [[dict valueForKey:@"MagazineIndex"] intValue];
	
	NSString *fPath = [libModel.contentList objectAtIndex:index];
	NSDictionary *pathDict = [NSDictionary dictionaryWithObject:[fPath stringByDeletingLastPathComponent] forKey:@"MagazinePath"];
	[[NSNotificationCenter defaultCenter] postNotificationName:@"LoadMagazineAtPath" object:self userInfo:pathDict];
	//Remove the library view
	[self.navigationController dismissModalViewControllerAnimated:YES];
}

#pragma mark UIScrollView delegate methods
- (void)scrollViewDidScroll:(UIScrollView *)sender {
	int pageCount = [libModel.contentList count]/9;
	if(pageCount == 0)
		pageCount = 1;
	
	CGFloat pageWidth = mScrollView.frame.size.width;
	float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    if(fractionalPage < 0)
		return;
	
	NSInteger lowerNumber = floor(fractionalPage);
	NSInteger upperNumber = lowerNumber + 1;
    
	if (lowerNumber == currentPage.pageIndex) {//User scrolling forward
		if (upperNumber != nextPage.pageIndex && upperNumber < pageCount) {
			[self applyNewIndex:upperNumber toPageController:nextPage];
			[self showPageAtIndex:upperNumber inController:nextPage];
		}
	}
	else if (upperNumber == currentPage.pageIndex) {//User scrolling backward
		if (lowerNumber != nextPage.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber toPageController:nextPage];
			[self showPageAtIndex:lowerNumber inController:nextPage];
		}
	}
	else {
		if(lowerNumber == nextPage.pageIndex) {
			[self applyNewIndex:upperNumber toPageController:currentPage];
		}
		else if(upperNumber == nextPage.pageIndex) {
			[self applyNewIndex:lowerNumber toPageController:currentPage];
		}
		else {
			[self applyNewIndex:lowerNumber toPageController:currentPage];
			[self applyNewIndex:upperNumber toPageController:nextPage];
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView {
	CGFloat pageWidth = mScrollView.frame.size.width;
    float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    NSInteger nearestNumber = lround(fractionalPage);
	
    if (currentPage.pageIndex != nearestNumber) {
        LibraryPageViewController *swapPage = currentPage;
        currentPage = nextPage;
        nextPage = swapPage;
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView {
	[self scrollViewDidEndScrollingAnimation:mScrollView];
}


@end
