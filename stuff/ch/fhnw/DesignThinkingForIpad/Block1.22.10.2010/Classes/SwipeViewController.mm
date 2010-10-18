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
		itemArea.size.height = 800;
		
		// initialize the scroll view with the dimension of the iPad screen
		mScrollView = [[UIScrollView alloc] initWithFrame:itemArea];
		
		// allow paging
		mScrollView.pagingEnabled = YES;
		
		// set bouncing of scoll view in horizontal direction:
		mScrollView.bounces = YES;
		
		// allow bouncing of scoll view in all directions:
		mScrollView.alwaysBounceVertical = NO;
		
		[self.view addSubview:mScrollView];
		
		
		// start initialization of the scroll view:
		[self initImageSwipeViews];
		
    }
    return self;
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
