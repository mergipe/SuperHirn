//
//  SwipeViewController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "SwipeViewController.h"

#import <QuartzCore/QuartzCore.h>

@implementation SwipeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		bottomNavViewController = [[BottomNavigationViewController alloc] initWithNibName:@"BottomNavigationView" bundle:nil];
		bottomNavViewController.delegate = self;

		self.view.backgroundColor = [UIColor blackColor];
		[self setUpSwipeView];
		[self setupBottomNavigationView];
		
		UIBarButtonItem *currentIssueItem = [[UIBarButtonItem alloc] initWithTitle:@"Show Navigation" style:UIBarButtonItemStyleBordered 
																			target:self action:@selector(hideBottomNavigation)];
		self.navigationItem.rightBarButtonItem = currentIssueItem;
		[currentIssueItem release];
		

					
    }
    return self;
}




- (void)setUpSwipeView 
{
	int nbItems = [allSwipeItems count];
	mScrollView.contentSize = CGSizeMake(nbItems * mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	CGRect itemFrame = mScrollView.frame;
	for( int i=0; i < nbItems;i++)
	{
		UIView* newItem = [allSwipeItems objectAtIndex:i];	
		newItem.frame = itemFrame;
		[ mScrollView addSubview:newItem];		
		itemFrame.origin.x += itemFrame.size.width;
	}
	
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	currentItemInView = 0;
}

-(void)addSwipeItem:(UIView*)iView
{
	if( allSwipeItems == nil )
	{
		allSwipeItems = [[NSMutableArray alloc] init];
	}	
	[allSwipeItems addObject:iView];
	[bottomNavViewController addNavItem:@"Test Item"];
}

- (void)setTitle:(NSString*) iTitle
{
	title = iTitle;
}


- (void)setImagename:(NSString*) iImagename;
{
	imagename = iImagename;
}

- (NSString*)title
{
	return title;
}

- (NSString*)imagename
{
	return imagename;
}

- (void)setupBottomNavigationView 
{
	//Setup bottom navigation
	CGRect frame = bottomNavViewController.view.frame;
	frame.size.width = self.view.frame.size.width;
	frame.origin.y = self.view.frame.size.height;
	//frame.origin.y = 0;
	bottomNavViewController.view.frame = frame;
	[self.view addSubview:bottomNavViewController.view];
	bottomNavViewController.view.hidden = YES;
}


-(void) centerAtItem:(int)iItemIndex
{
	currentItemInView = iItemIndex;
	[ mScrollView setContentOffset:CGPointMake(currentItemInView * mScrollView.frame.size.width, 0.0) animated:YES];
}


#pragma mark UIScrollView delegate methods


- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView 
{
	CGFloat pageWidth = mScrollView.frame.size.width;
	int pageNumber = floor((mScrollView.contentOffset.x - pageWidth/2) / pageWidth) + 1;
	NSLog(@"ScrollView swiped to page %d", pageNumber );
	currentItemInView = pageNumber;
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
	
	// initialize the scroll view with the dimension of the iPad screen
	mScrollView = [[UIScrollView alloc] initWithFrame:self.view.frame];
	
	// allow paging
	mScrollView.pagingEnabled = YES;
	
	// set bouncing of scoll view in horizontal direction:
	mScrollView.bounces = YES;
	
	// allow bouncing of scoll view in all directions:
	mScrollView.alwaysBounceVertical = NO;
	
	// set self as the delegated for the scroll view
	mScrollView.delegate = self;
	
	// add the scroll view as a subview
	[self.view addSubview:mScrollView];
	

}

#pragma mark BottomNavigationDelegate methods
- (void)tappedOnBottomNavigation:(BottomNavigationViewController *)viewController onItemAtIndex:(int)index 
{
	[mScrollView setContentOffset:CGPointMake(index * mScrollView.frame.size.width, 0.0) animated:YES];

}

- (void)showBottomNavigation 
{
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.3];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	
	CGRect f = bottomNavViewController.view.frame;
	f.size.width = self.view.frame.size.width;
	bottomNavViewController.view.frame = f;
	bottomNavViewController.view.center = CGPointMake(f.size.width/2, self.view.frame.size.height - f.size.height/2);
	bottomNavViewController.view.hidden = NO;
	[UIView commitAnimations];
	
	self.navigationItem.rightBarButtonItem.title = @"Hide Navigation";
	self.navigationItem.rightBarButtonItem.action = @selector(hideBottomNavigation);

}

- (void)hideBottomNavigation 
{
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.3];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	
	CGRect f = bottomNavViewController.view.frame;
	f.size.width = self.view.frame.size.width;
	bottomNavViewController.view.frame = f;
	bottomNavViewController.view.center = CGPointMake(f.size.width/2, self.view.frame.size.height + f.size.height/2);
	bottomNavViewController.view.hidden = NO;
	
	[UIView commitAnimations];
	
	
	self.navigationItem.rightBarButtonItem.title = @"Show Navigation";
	self.navigationItem.rightBarButtonItem.action = @selector(showBottomNavigation);
	
}

- (void)dealloc 
{
	[allSwipeItems release];
	[mScrollView release];
	[super dealloc];
}


@end
