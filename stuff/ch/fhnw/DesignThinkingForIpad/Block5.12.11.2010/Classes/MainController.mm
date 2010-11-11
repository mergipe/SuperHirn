//
//  MainController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "MainController.h"
#import "SwipeViewController.h"
#import "ItemViewController.h"
#import "StudentController.h"
#import "ListViewController.h"

#import "ModuleInfoView.h"
#import "ItemWebView.h"

#import <QuartzCore/QuartzCore.h>


@implementation MainController

- (id) initWithRootViewController:(UIViewController*)ignored
{
	rootController = [[ItemViewController alloc] initWithNibName:@"ItemViewController" bundle:nil];
	if (self = [super initWithRootViewController:rootController]) 
	{

		rootController.delegate = self;

		
		SwipeViewController* swipe = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[swipe setTitle:@"Module Info"];
		[self addController:swipe];
		ItemWebView* item = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item setUrl:@"Modulbeschreibung_analysis.pdf"];
		[swipe addSwipeItem:item];
		ItemWebView* item2 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item2 setUrl:@"Modulbeschreibung_design.pdf"];
		[swipe addSwipeItem:item2];
		[swipe setUpSwipeView];
		[swipe showBottomNavigation];
		
		
		
		
		
		SwipeViewController* swipe2 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[swipe2 setTitle:@"Time Schedule"];
		[self addController:swipe2];
		ItemWebView* item3 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item3 setUrl:@"Modulbeschreibung_design.pdf"];
		[item3 setUrl:@"html/example.html"];
		[swipe2 addSwipeItem:item3];
		
		[swipe2 setUpSwipeView];
		[swipe2 showBottomNavigation];
		[swipe2 hideBottomNavigation];
		
		
		
	
		SwipeViewController* swipe3 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[swipe3 setTitle:@"Contacts"];
		[self addController:swipe3];
		list = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];		
		CGRect f1 = CGRectMake(0,0,768,1000);
		list.view.frame = f1;
		[swipe3 addSwipeItem:list.view];
		[swipe3 setUpSwipeView];
		[swipe3 hideBottomNavigation];
		
		
		
		
		
		////////////////////
		// initialize the a swipe view controller with all images:		
		SwipeViewController* swipe4 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[swipe4 setTitle:@"Gallery"];
		[self addController:swipe4];
		NSMutableArray* listOfPictures = [StudentController getFiles];
		CGRect f = CGRectMake(0,0,768,1000);
		for( int i=0; i < [listOfPictures count ];i++)
		{
			NSString* imageName = [listOfPictures objectAtIndex:i];
			PictureView* item = [[PictureView alloc] initWithFrame:f];
			[ item setItemIndex:i];
			[ item setImage:imageName];
			[ swipe4 addSwipeItem:item];
		}
		[swipe4 setUpSwipeView];
		[swipe4 showBottomNavigation];
		
	}
	return self;
}

 
-(void)addController:(SwipeViewController*)iController
{
	if( controllers == nil )
	{
		controllers = [[NSMutableArray alloc] init];
	}
	[controllers addObject:iController];
	[rootController addTapItem: ([controllers count] - 1) withTitle: [iController title] ];
}



- (void) showController:(int) iIndex
{
	NSLog(@"ItemViewShows: show swipe view at item %d.", iIndex);
	
	SwipeViewController* con = [controllers objectAtIndex:iIndex];
	
	// now push the swipe view in front:
	[self pushViewController:con animated:YES];
}



-(void)didSelectItem:(int)iItemIndex
{
	NSLog(@"Tapped on TapItem: %d", iItemIndex );
	[self showController:iItemIndex];
}


- (void)viewDidLoad 
{
	[super viewDidLoad];
}


- (void)dealloc 
{
	[controllers dealloc];
	[super dealloc];
}


@end
