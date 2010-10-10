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



@end
