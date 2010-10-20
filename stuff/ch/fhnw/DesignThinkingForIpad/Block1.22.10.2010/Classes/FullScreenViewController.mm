//
//  FullScreenViewController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "FullScreenViewController.h"
#import "StudentController.h"
#import "PictureView.h"
#import "ItemWebView.h"

@implementation FullScreenViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		self.view.backgroundColor = [UIColor blackColor]; 
		imagePositionX = 0;
		imagePositionY = 0;
		imageWidth = 768;
		imageHeight = 1024;
		
    }
    return self;
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
	

	// Get the file name of the image, set the image in Pictures and add a border:
	NSString* imageName = [StudentController getFile:iIndex];	
	[ mPicture setImage:imageName ];
	[ mPicture addBorder ];
	
	

	
}

/*
- (void)showWebItem 
{
	ItemWebView* mWebItem = nil;
	if( mWebItem == nil )
	{
		mWebItem = [[ItemWebView alloc] initWithFrame:CGRectMake(
																 imagePositionX, imagePositionY, 
																 imageWidth, imageHeight)];
		//[mPicture setFullScreen: YES ];
		[self.view addSubview: mWebItem ];
		
	}

	[mWebItem setUrl:@"test.pdf"];	
	
}
 */

- (void)viewDidLoad 
{
	[super viewDidLoad];
	
}


- (void)dealloc 
{
	[mPicture release];
	[super dealloc];
}



@end
