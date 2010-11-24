//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "BottomNavigationViewController.h"


@implementation BottomNavigationViewController

@synthesize delegate;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
        // Custom initialization
		navItems = [[NSMutableArray alloc] init];
		ItemSpacing = 194;
		ScrollViewHeight = 194;

    }
    return self;
}

- (void)dealloc 
{
	[super dealloc];
	[navItems release];
}

- (void)viewDidLoad 
{
	[super viewDidLoad];
	self.view.backgroundColor = [UIColor clearColor];
}


- (void)addNavItem:(NSString*) iTitle
{
	
	int count = [navItems count];
	mScrollView.contentSize = CGSizeMake(ItemSpacing * (count + 1), ScrollViewHeight);
	
	int x = ItemSpacing * count;
	BottomNavigationItemView *v = [[BottomNavigationItemView alloc] initWithFrame:CGRectMake(x, 0.0, ItemSpacing, ScrollViewHeight)];
	v.delegate = self;
	v.titleLabel.text = iTitle;
	//v.iconView.image = [UIImage imageWithContentsOfFile:iconPath];	
	
	
	[mScrollView addSubview:v];
	fScrollWidth = (x + ItemSpacing);	
	[navItems addObject:v];
	[v release];
}


- (void)addNavItemX:(NSString*) iTitle :(int)i
{
	
	int count = [navItems count];
	mScrollView.contentSize = CGSizeMake(ItemSpacing * (count + 1), ScrollViewHeight);
	
	int x = ItemSpacing * count;
	BottomNavigationItemView *v = [[BottomNavigationItemView alloc] initWithFrameX:CGRectMake(x, 0.0, ItemSpacing, ScrollViewHeight): i];
	v.delegate = self;
	v.titleLabel.text = iTitle;
	//v.iconView.image = [UIImage imageWithContentsOfFile:iconPath];	
	
	
	[mScrollView addSubview:v];
	fScrollWidth = (x + ItemSpacing);	
	[navItems addObject:v];
	[v release];
}

- (void)centerToIndex:(int)iIndex 
{
	CGFloat xOffset = 0;
	int width = self.view.frame.size.width;
	if( iIndex == 0 )
	{
		xOffset = 0;
	}
	else if( iIndex == ([navItems count] - 1) )
	{
		xOffset = [navItems count] * ItemSpacing - ItemSpacing/2.0;
	}
	else
	{
		xOffset = ItemSpacing * (iIndex + 1) - ItemSpacing/2.0 - width/2.0;
	}

	if( iIndex >= 0  )
	{
		int width = self.view.frame.size.width;
		CGFloat viewedXDistance = fScrollWidth - xOffset;
		if( viewedXDistance < width )
		{
			xOffset -= width - viewedXDistance; 
		}
		
		if( xOffset < 0 )
		{
			xOffset = 0;
		}		
		[mScrollView setContentOffset:CGPointMake( xOffset , 0.0) animated:YES];
	}
}


#pragma mark BottomNavigationItemView delegate methods
- (void)tappedOnItem:(UIView *)view {
	int index = [navItems indexOfObject:view];
	NSLog(@"Tapped on bottom item at index: %d", index);
	if(index >= 0 && [delegate respondsToSelector:@selector(tappedOnBottomNavigation:onItemAtIndex:)]) 
	{
		[delegate tappedOnBottomNavigation:self onItemAtIndex:index];
	}
}

@end
