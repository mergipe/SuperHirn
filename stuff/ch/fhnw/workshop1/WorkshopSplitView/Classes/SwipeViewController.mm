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

@implementation SwipeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		imageArea.origin.x = 0;
		imageArea.origin.y = 100;
		imageArea.size.width = 768;
		imageArea.size.height = 800;
		
		// initialize the scroll view with the dimension of the iPad screen
		CGSize iPadScreenSize = self.view.frame.size;
		mScrollView = [[UIScrollView alloc] initWithFrame:imageArea];
					   //CGRectMake(0.0, 0.0, iPadScreenSize.width, iPadScreenSize.height)];

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
		[self initImageSwipeViews];
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
	[super viewDidLoad];
	//self.navigationController.navigationBar.hidden = YES;	
	/*
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.4];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	
	CGRect f = mScrollView.frame;
	int h = mScrollView.frame.size.height;
	int y = 700 + h * 0.5;
	mScrollView.center = CGPointMake(f.size.width/2, y);
	
	[UIView commitAnimations];
	*/
	
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
		//item.delegate = self;		
		UIImage* _image = [UIImage imageNamed:imageName];
		[ item setImage:_image];
		[ item setTitle: imageName];
		
		[mScrollView addSubview:item];		
		
		f.origin.x += f.size.width;
	}
	
}


/*
- (void) showThommenBackgroundImageOnStart
{
	CGRect frame = self.view.frame;
	backgroundImage = [[UIImageView alloc] 
					   initWithFrame:frame];
	backgroundImage.image = [UIImage imageNamed:@"Default.png"];
	[self.view addSubview: backgroundImage];	
}
 */


-(void)fadeInWebView:(UIWebView *)webView 
{

	/*
	webView.alpha = 0.0;
	webView.hidden = NO;
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.25];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	webView.alpha = 1.0;
	[UIView commitAnimations];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:1.0];
	[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
	backgroundImage.alpha = 0.0;
	backgroundImage.hidden = NO;
	[UIView commitAnimations];	
	 */
}


/*
 - (void)showNavigationBar 
{
	int yBar = mBottomNav.view.frame.origin.y;
	int hScreen = self.view.frame.size.height;
	
	if( yBar == hScreen ) 
	{
		[mBottomNav showNavigationBar];

		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.4];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];

		CGRect f = mBottomNav.view.frame;
		int h = mBottomNav.view.frame.size.height;
		int y = hScreen - h * 0.5;
		mBottomNav.view.center = CGPointMake(f.size.width/2, y);
		
		[UIView commitAnimations];
		

	}
	
}

- (void)hideNavigationBar 
{
	int yBar = mBottomNav.view.frame.origin.y;
	int hScreen = self.view.frame.size.height;
	if( yBar != hScreen ) 
	{
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.4];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
		
		CGRect f = mBottomNav.view.frame;
		int h = mBottomNav.view.frame.size.height;
		int y = hScreen + h * 0.5;
		mBottomNav.view.center = CGPointMake(f.size.width/2, y);
		
		[UIView commitAnimations];
		
		[mBottomNav performSelector:@selector(hideNavigationBar) withObject:nil afterDelay:0.4];
	}
	
}
 */





#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point 
{	
}

#pragma mark UIScrollView delegate methods
- (void)scrollViewDidScroll:(UIScrollView *)sender 
{
}
- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView
{	
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView 
{
}




/*****
 *
 * Here follow not implemented and commented methods from UIScrollViewDelegate and UIViewController
 */

/*
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
 {
 return NO;
 }
 - (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation 
 {	
 }
 - (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
 {
 }
 
 - (void)viewWillAppear:(BOOL)animated 
 {
 [super viewWillAppear:animated];
 }
 
 - (void)viewDidAppear:(BOOL)animated 
 {
 }
 
 - (void)viewWillDisappear:(BOOL)animated 
 {
 [super viewWillDisappear:animated];
 }
 
 - (void)viewDidUnload 
 {
 [super viewDidUnload];
 }
 */


@end
