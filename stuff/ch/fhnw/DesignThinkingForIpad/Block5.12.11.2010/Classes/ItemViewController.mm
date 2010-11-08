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

@synthesize delegate;

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
					
    }
    return self;
}


- (void)addTapItem 
{
	
	int x = 30;
	int y = 30;
	int index = 0;
	CGRect area = CGRectMake(x, y, 300, 300 );
	
	TapIcon* item = [[TapIcon alloc] initWithFrame:area];
	[ item setItemIndex: index ];
	item.delegate = self;
	
	int tmp = index / 2.0;
	
	area.origin.x += x + area.size.width + 30;
	area.origin.y += y + area.size.height + 30;
	area.origin.x = 30;	
	
	[self.view addSubview: item ];
	
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


-(void)tapOnItem:(int)iItemIndex
{
	NSLog(@"Tapped on TapItem: %d", iItemIndex );
	[ delegate didSelectItem:iItemIndex];
}


@end
