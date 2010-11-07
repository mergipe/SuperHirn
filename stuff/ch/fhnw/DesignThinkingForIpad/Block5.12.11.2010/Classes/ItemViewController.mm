//
//  ItemViewController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "ItemViewController.h"


#import "TapIcon.h"
#import <QuartzCore/QuartzCore.h>


@implementation ItemViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		// make a gradient background
		CAGradientLayer *gradient = [CAGradientLayer layer];
		gradient.frame = self.view.frame;
		UIColor *startColor = [UIColor colorWithWhite: 0.5 alpha: 1.0];
		UIColor *endColor = [UIColor blackColor];
		gradient.colors = [NSArray arrayWithObjects:(id)[startColor CGColor], (id) [endColor CGColor], nil];
		[self.view.layer insertSublayer:gradient atIndex:1];
					
		// start initialization of the scroll view:
		[self initItemView];
		
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
	[super viewDidLoad];
}


- (void)dealloc 
{
	[super dealloc];
}

- (void)initItemView 
{

	int x = 30;
	int y = 30;
	CGRect area = CGRectMake(x, y, 300, 300 );

	TapIcon* item1 = [[TapIcon alloc] initWithFrame:area];
	[item1 setItemIndex:0];
	item1.delegate = self;
	[self.view addSubview: item1 ];

	area.origin.x += x + area.size.width + 30;
	TapIcon* item2 = [[TapIcon alloc] initWithFrame:area];
	[item2 setItemIndex:1];
	item2.delegate = self;
	[self.view addSubview: item2 ];

	area.origin.y += y + area.size.height + 30;
	area.origin.x = 30;
	TapIcon* item3 = [[TapIcon alloc] initWithFrame:area];
	[item3 setItemIndex:2];
	item3.delegate = self;
	[self.view addSubview: item3 ];

	area.origin.x += x + area.size.width + 30;
	TapIcon* item4 = [[TapIcon alloc] initWithFrame:area];
	[item4 setItemIndex:3];
	item4.delegate = self;
	[self.view addSubview: item4 ];
	
}

-(void)tapOnItem:(int)iItemIndex
{
	NSLog(@"Tapped on TapItem: %d", iItemIndex );
}


@end
