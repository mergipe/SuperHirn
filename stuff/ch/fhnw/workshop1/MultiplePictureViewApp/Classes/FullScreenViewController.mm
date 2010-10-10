//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//

#import "FullScreenViewController.h"
#import "StudentController.h"
#import "PictureView.h"

#import <QuartzCore/QuartzCore.h>

@implementation FullScreenViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		// make a gradient background
		/*
		CAGradientLayer *gradient = [CAGradientLayer layer];
		gradient.frame = self.view.frame;
		UIColor *startColor = [UIColor colorWithWhite: 0.5 alpha: 1.0];
		UIColor *endColor = [UIColor blackColor];
		gradient.colors = [NSArray arrayWithObjects:(id)[startColor CGColor], (id) [endColor CGColor], nil];
		[self.view.layer insertSublayer:gradient atIndex:1];
		*/
		
		self.view.backgroundColor = [UIColor blackColor]; 
		imagePositionX = 0;
		imagePositionY = 0;
		imageWidth = 768;
		imageHeight = 800;
		
		[self showPicture:0];
    }
    return self;
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
	
}


- (void)dealloc 
{
	[mPicture release];
	[super dealloc];
}

- (void)showPicture:(int) iIndex 
{
	if( mPicture == nil )
	{
		mPicture = [[PictureView alloc] initWithFrame:CGRectMake(
																 imagePositionX, imagePositionY, 
																 imageWidth, imageHeight)];
		[mPicture setFullScreen: YES ];
		[self.view addSubview: mPicture ];

	}
	
	NSString* imageName = [StudentController getPicture:iIndex];	
	[ mPicture setImage:imageName ];
	
	
}



#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point 
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
