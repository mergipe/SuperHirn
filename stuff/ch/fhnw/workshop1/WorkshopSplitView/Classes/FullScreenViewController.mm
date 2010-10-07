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


@implementation FullScreenViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		imagePositionX = 0;
		imagePositionY = 100;
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
		[self.view addSubview: mPicture ];

	}
	
	NSString* imageName = [StudentController getPicture:iIndex];	
	UIImage* _image = [UIImage imageNamed:imageName];
	
	[ mPicture setImage:_image ];
	[ mPicture setTitle: imageName ];
	
	
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
