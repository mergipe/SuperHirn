    //
//  LibraryPageViewController.m
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "LibraryPageViewController.h"

#import "LibraryItemView.h"
#import "LibraryModel.h"

@implementation LibraryPageViewController

@synthesize pageIndex;


 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		libraryItems = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc {
	[super dealloc];
	[libraryItems release];
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)loadLibraryForPageAtIndex:(int)index {
	//Find out the start and end index
	LibraryModel *libModel = [LibraryModel sharedModel];
	
	int sIndex = index * 9;
	int eIndex = sIndex + 9;
	if(eIndex > [libModel.contentList count]) {
		eIndex = [libModel.contentList count] - sIndex;
	}
	int libItemIndex = 0;
	int viewX = 50.0;
	int viewY = 50.0;
	
	for(int i = sIndex; i < eIndex; i++) {
		LibraryItemView *itemView;
		//First try to use the existing library items
		if([libraryItems count] > libItemIndex) {
			itemView = [libraryItems objectAtIndex:libItemIndex];
		}
		else {
			//Create a new one and find its location/frame
			itemView = [[LibraryItemView alloc] initWithFrame:CGRectMake(viewX, viewY, 175.0, 250.0)];
			itemView.delegate = self;
			[self.view addSubview:itemView];
			[libraryItems addObject:itemView];
			[itemView release];
		}

		itemView.thumbnailView.image = [UIImage imageWithContentsOfFile:[libModel.contentList objectAtIndex:i]];
		itemView.issueLabel.text = [libModel.issueList objectAtIndex:i];
		itemView.titleLabel.text = [libModel.titleList objectAtIndex:i];
		
		if(libItemIndex > 0 && (libItemIndex+1)%3 == 0) {
			//increment y
			viewY += 300.0;
			//reset x
			viewX = 50.0;
		}
		else {
			//increment x
			viewX += 200.0;
		}
		libItemIndex++;
	}
	//Remove the extra item views if there are any
	for(int j = libItemIndex; j < [libraryItems count]; j) {
		LibraryItemView *itemView = [libraryItems objectAtIndex:j];
		[itemView removeFromSuperview];
		[libraryItems removeObjectAtIndex:j];
	}
}

#pragma mark BottomNavigationItemView delegate methods
- (void)tappedOnItem:(UIView *)view {
	NSLog(@"Tapped on library item");
	int index = [libraryItems indexOfObject:view];
	if(index >= 0) {
		int i = 9*pageIndex + index;
		NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:i] forKey:@"MagazineIndex"];
		[[NSNotificationCenter defaultCenter] postNotificationName:@"LoadMagazineAtIndex" object:self userInfo:dict];
	}
}

@end
