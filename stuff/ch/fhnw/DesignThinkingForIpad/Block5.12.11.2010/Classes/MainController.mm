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

		
		SwipeViewController* moduleInfo = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[moduleInfo setTitle:@"Module Info"];
		[self addController:moduleInfo];
		ItemWebView* item = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item setUrl:@"Modulbeschreibung_analysis.pdf"];
		[moduleInfo addSwipeItem:item];
		ItemWebView* item2 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item2 setUrl:@"pdf/test 14.pdf"];
		[moduleInfo addSwipeItem:item2];
		[moduleInfo setUpSwipeView];
		[moduleInfo showBottomNavigation];
		
		
		
		
		
		SwipeViewController* timeSchedule = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[timeSchedule setTitle:@"Time Schedule"];
		[self addController:timeSchedule];
		ItemWebView* item3 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item3 setUrl:@"html/example.html"];
		[timeSchedule addSwipeItem:item3];
		
		[timeSchedule setUpSwipeView];
		[timeSchedule showBottomNavigation];
		[timeSchedule hideBottomNavigation];
		
		
		
	
		SwipeViewController* contacts = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[contacts setTitle:@"Contacts"];
		[self addController:contacts];
		list = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];		
		CGRect f1 = CGRectMake(0,0,768,1000);
		list.view.frame = f1;
		[contacts addSwipeItem:list.view];
		[contacts setUpSwipeView];
		[contacts hideBottomNavigation];
		
		
		
		SwipeViewController* gallery = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[gallery setTitle:@"Gallery"];
		[self addController:gallery];
		NSMutableArray* listOfPictures = [StudentController getFiles];
		CGRect f = CGRectMake(0,0,768,1000);
		for( int i=0; i < [listOfPictures count ];i++)
		{
			NSString* imageName = [listOfPictures objectAtIndex:i];
			PictureView* item = [[PictureView alloc] initWithFrame:f];
			[ item setItemIndex:i];
			[ item setImage:imageName];
			[ gallery addSwipeItem:item];
		}
		[gallery setUpSwipeView];
		[gallery showBottomNavigation];
		
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
