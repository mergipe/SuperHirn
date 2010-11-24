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
#import "PDFView.h"

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
		[moduleInfo setImagename:@"module.png"];

		[self addController:moduleInfo];
		
		
		NSMutableArray* listOfPdf = [StudentController getPdfs:@"Data/pdf/"];
		
		for( int i=0; i < [listOfPdf count ];i++)
		{
			NSString* pdfName = [listOfPdf objectAtIndex:i];			
			PDFView* item = [[PDFView alloc] initWithFrame: CGRectMake(0,0,768,800)];
			[ item setItemIndex:i];
			[ item setPdf: pdfName];
			[moduleInfo addSwipeItem:item: @"Data/icon/Analysis.jpg" :i];
		}
		
		[moduleInfo setUpSwipeView];
		[moduleInfo showBottomNavigation];
		
		
		
		SwipeViewController* timeSchedule = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[timeSchedule setTitle:@"Time Schedule"];
		[timeSchedule setImagename:@"schedule4.png"];
		[self addController:timeSchedule];
		ItemWebView* item4 = [[ItemWebView alloc] initWithFrame: CGRectMake(0,0,768,1024) ];
		[item4 setUrl:@"html/example.html"];
		[timeSchedule addSwipeItem:item4];
		
		[timeSchedule setUpSwipeView];
		[timeSchedule showBottomNavigation];
		[timeSchedule hideBottomNavigation];
		
		
		
	
		SwipeViewController* contacts = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[contacts setTitle:@"Contacts"];
		[contacts setImagename:@"contacts.png"];
		[self addController:contacts];
		list = [[ListViewController alloc] initWithNibName:@"ListViewController" bundle:nil];		
		CGRect f1 = CGRectMake(0,0,768,1000);
		list.view.frame = f1;
		[contacts addSwipeItem:list.view];
		[contacts setUpSwipeView];
		[contacts hideBottomNavigation];
		
		
		
		
		GalleryController* gallery = [[GalleryController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[gallery setTitle:@"Gallery"];
		[gallery setImagename:@"photos_icon.png"];
		[self addController:gallery];
		NSMutableArray* listOfPictures = [StudentController getImageFiles];
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
		[Muster1 setTitle:@"more coming soon"];
		[Muster1 setImagename:@"Wizard.png"];
		[self addController:Muster1];
				
		SwipeViewController* Muster2 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[Muster2 setTitle:@"more coming soon"];
		[Muster2 setImagename:@"Wizard.png"];
		[self addController:Muster2];
		
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
	[rootController addTapItem: ([controllers count] - 1) withTitle: [iController title] : [iController imagename]];
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
