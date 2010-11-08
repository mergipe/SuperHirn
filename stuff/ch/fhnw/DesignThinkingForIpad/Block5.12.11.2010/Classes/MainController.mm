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

#import <QuartzCore/QuartzCore.h>


@implementation MainController

- (id) initWithRootViewController:(UIViewController*)ignored
{
	rootController = [[ItemViewController alloc] initWithNibName:@"ItemViewController" bundle:nil];
	if (self = [super initWithRootViewController:rootController]) 
	{

		rootController.delegate = self;

		SwipeViewController* swipe = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[self addController:swipe];
		SwipeViewController* swipe2 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[self addController:swipe2];
	
		SwipeViewController* swipe3 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[self addController:swipe3];
		SwipeViewController* swipe4 = [[SwipeViewController alloc] initWithNibName:@"SwipeViewController" bundle:nil];
		[self addController:swipe4];
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
	[rootController addTapItem: [controllers count] - 1 ];
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
