//
//  SwipeViewController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "SwipeViewController.h"
#import "StudentController.h"
#import "PictureView.h"

#import <QuartzCore/QuartzCore.h>


@implementation SwipeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		// make a gradient background
		CAGradientLayer *gradient = [CAGradientLayer layer];
		gradient.frame = self.view.frame;
		UIColor *startColor = [UIColor colorWithWhite: 0.5 alpha: 1.0];
		UIColor *endColor = [UIColor blackColor];
		gradient.colors = [NSArray arrayWithObjects:(id)[startColor CGColor], (id) [endColor CGColor], nil];
		[self.view.layer insertSublayer:gradient atIndex:1];
		
		// defines the position and size of each item displayed  
		itemArea.origin.x = 0;
		itemArea.origin.y = 0;
		itemArea.size.width = 768;
		itemArea.size.height = 1024;
		
		// initialize the scroll view with the dimension of the iPad screen
		mScrollView = [[UIScrollView alloc] initWithFrame:itemArea];
		
		// allow paging
		mScrollView.pagingEnabled = YES;
		
		// set bouncing of scoll view in horizontal direction:
		mScrollView.bounces = YES;
		
		// allow bouncing of scoll view in all directions:
		mScrollView.alwaysBounceVertical = NO;
		
		// set self as the delegated for the scroll view
		mScrollView.delegate = self;
		
		[self.view addSubview:mScrollView];
		
		
		// start initialization of the scroll view:
		[self initImageSwipeViews];
		
    }
    return self;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration 
{
	
	CGSize newSize = CGSizeMake( itemArea.size.height, itemArea.size.width);
	mScrollView.frame = CGRectMake(0, 0, newSize.width, newSize.height);
	mScrollView.contentSize = CGSizeMake( [StudentController numberOfFiles] * newSize.width, newSize.height);
	
	[self centerAtItem:3];
		
	itemArea.size.width = mScrollView.frame.size.height;
	itemArea.size.height = mScrollView.frame.size.width;
}


- (void)initImageSwipeViews 
{
	// initialize the slide with the number of pics:
	listOfPictures = [StudentController getFiles];
	mScrollView.contentSize = CGSizeMake([listOfPictures count] * itemArea.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	CGRect f = itemArea;
	for( int i=0; i < [listOfPictures count ];i++)
	{
		NSString* imageName = [listOfPictures objectAtIndex:i];
		PictureView* item = [[PictureView alloc] initWithFrame:f];
		[ item setItemIndex:i];
		[ item setImage:imageName];
		[ item addBorder];
		
		[mScrollView addSubview:item];		
		f.origin.x += f.size.width;
	}
}


-(void) centerAtItem:(int)iItemIndex
{
	[ mScrollView setContentOffset:CGPointMake(iItemIndex * mScrollView.frame.size.width, 0.0) animated:YES];
}


#pragma mark UIScrollView delegate methods

- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView 
{
	CGFloat pageWidth = mScrollView.frame.size.width;
	int pageNumber = floor((mScrollView.contentOffset.x - pageWidth/2) / pageWidth) + 1;
	NSLog(@"ScrollView swiped to page %d", pageNumber );
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
}


- (void)dealloc 
{
	[listOfPictures release];
	[mScrollView release];
	[super dealloc];
}


@end
