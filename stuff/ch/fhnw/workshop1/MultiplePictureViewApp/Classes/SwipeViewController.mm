//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
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
		
		
		imageArea.origin.x = 0;
		imageArea.origin.y = 0;
		imageArea.size.width = 768;
		imageArea.size.height = 800;
		
		// initialize the scroll view with the dimension of the iPad screen
		CGSize iPadScreenSize = self.view.frame.size;
		mScrollView = [[UIScrollView alloc] initWithFrame:imageArea];
		
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

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
	[super viewDidLoad];
}




- (void)dealloc 
{
	[mScrollView release];
	[super dealloc];
}

- (void)initImageSwipeViews 
{
	
	
	// initialize the slide with the number of pics:
	listOfPictures = [StudentController getPictures];
	mScrollView.contentSize = CGSizeMake([listOfPictures count] * imageArea.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	CGRect f = imageArea;
	for( int i=0; i < [listOfPictures count ];i++)
	{
		NSString* imageName = [listOfPictures objectAtIndex:i];
		PictureView* item = [[PictureView alloc] initWithFrame:f];
		[ item setPictureIndex:i];
		[ item setImage:imageName];
		
		[mScrollView addSubview:item];		
		
		f.origin.x += f.size.width;
	}
	
	self.tabBarItem;

}




@end
