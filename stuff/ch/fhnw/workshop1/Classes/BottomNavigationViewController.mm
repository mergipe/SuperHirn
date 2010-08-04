//
//  AkiliPad
//
//  Created by Mithin on 04/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "BottomNavigationViewController.h"

#import "NavigationModel.h"
#import "NCXNavPoint.h"
#import "AkiliLabel.h"

@implementation BottomNavigationViewController


// The designated initializer.  Override if you create the controller programmatically and 
//want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
		navItems = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc {
	[super dealloc];
	[navItems release];
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	
	NavigationModel *navModel = [NavigationModel sharedModel];
	const QList<NCXNavPoint *> &navPoints  = [navModel topLevelNavPointList];
	mScrollView.contentSize = CGSizeMake(220.0 * navPoints.count(), 150.0);
	int x = 0;
	for(int i = 0; i < navPoints.count(); i++) {
		NCXNavPoint *navPoint = navPoints[i];
		BottomNavigationItemView *v = [[BottomNavigationItemView alloc] initWithFrame:CGRectMake(x, 0.0, 220.0, 150.0)];
		v.delegate = self;
		//v.backgroundColor = [UIColor whiteColor];
		v.titleLabel.text = [NSString stringWithUTF8String:navPoint->title().toUtf8().data()];
		//v.subTitleLabel.text = [NSString stringWithUTF8String:navPoint->subTitle().toUtf8().data()];
		NSString *iconPath = [navModel filePathForItemWithId:[NSString stringWithUTF8String:navPoint->iconID().toUtf8().data()]];
		v.iconView.image = [UIImage imageWithContentsOfFile:iconPath];
		
		x += 220.0;
		[navItems addObject:v];
		[mScrollView addSubview:v];
		
		[v release];
	}
}

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

#pragma mark BottomNavigationItemView delegate methods
- (void)tappedOnItem:(UIView *)view {
	NSLog(@"Tapped on bottom item");
	int index = [navItems indexOfObject:view];
	if(index >= 0) {
		NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:index] forKey:@"BottomNavIndex"];
		[[NSNotificationCenter defaultCenter] postNotificationName:@"LoadArticleWithBottomNavIndex" object:self userInfo:dict];
	}
}

@end
