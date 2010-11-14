//
//  MainController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "MainController.h"
#import "GalleryController.h"
#import "SwipeViewController.h"
#import "ItemViewController.h"
#import "StudentController.h"
#import "ListViewController.h"
#import "ModuleController.h"

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
		
		
		ModuleController* moduleInfo = [[ModuleController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[moduleInfo setTitle:@"Module Info"];
		[self addController:moduleInfo];
		ItemWebView* item = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,800) ];
		[item setUrl:@"pdf/Sammelmappe_Modulplakate 1.pdf"];
		[moduleInfo addSwipeItem:item:@"Analysis": @"designProzesse1.jpg"];
		ItemWebView* item2 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,800) ];
		[item2 setUrl:@"pdf/Sammelmappe_Modulplakate 2.pdf"];
		[moduleInfo addSwipeItem:item2:@"Infg": @"file-icon.png"];
		ItemWebView* item3 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,800) ];
		[item3 setUrl:@"pdf/Modulbeschreibung_design.pdf"];
		[moduleInfo addSwipeItem:item3:@"Design": @"analysis1.jpg"];
		[moduleInfo setUpSwipeView];
		[moduleInfo showBottomNavigation];
		
		
		
		
		
		SwipeViewController* timeSchedule = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[timeSchedule setTitle:@"Time Schedule"];
		[self addController:timeSchedule];
		ItemWebView* item4 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item4 setUrl:@"html/example.html"];
		[timeSchedule addSwipeItem:item4];
		
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
		
		
		
		GalleryController* gallery = [[GalleryController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
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
			[ gallery addSwipeItemX:item ItemIndex:i];

		}
		[gallery setUpSwipeView];
		[gallery showBottomNavigation];
		
		
		
		SwipeViewController* Muster1 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[Muster1 setTitle:@""];
		[self addController:Muster1];
		list = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];		
		CGRect fm1 = CGRectMake(0,0,768,1000);
		list.view.frame = fm1;
		[Muster1 addSwipeItem:list.view];
		[Muster1 setUpSwipeView];
		[Muster1 hideBottomNavigation];
		
		
		SwipeViewController* Muster2 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[Muster2 setTitle:@""];
		[self addController:Muster2];
		list = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];		
		CGRect fm2 = CGRectMake(0,0,768,1000);
		list.view.frame = fm2;
		[Muster2 addSwipeItem:list.view];
		[Muster2 setUpSwipeView];
		[Muster2 hideBottomNavigation];
		
		
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
