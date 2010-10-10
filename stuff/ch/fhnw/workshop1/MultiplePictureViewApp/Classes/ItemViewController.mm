//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import "ItemViewController.h"


#import "StudentController.h"
#import "ItemPageView.h"

#import <QuartzCore/QuartzCore.h>


@implementation ItemViewController

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
		
		// set this class as the delegate (observer) of the scroll view and add is as a subview:
		mScrollView.delegate = self;		
		[self.view addSubview:mScrollView];
		
		
		// start initialization of the scroll view:
		[self initItemView];
		
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

- (void)initItemView 
{
	
	
	// initialize the slide with the number of pics:
	listOfPictures = [StudentController getPictures];
	int nbPages = [listOfPictures count] / [ItemPageView numberItemsPerPage] + 1;
	mScrollView.contentSize = CGSizeMake( nbPages * imageArea.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	CGRect f = imageArea;
	int i= 0;
	while( i < [listOfPictures count ])
	{
		ItemPageView* item = [[ItemPageView alloc] initWithFrame:f];
		[item setItemIndexes:i withEndIndex:i + [ItemPageView numberItemsPerPage] - 1];
		[mScrollView addSubview:item];	
		[item release];
	
		i += ItemPageView.numberItemsPerPage;
		f.origin.x += f.size.width;
	}
	
}

@end
