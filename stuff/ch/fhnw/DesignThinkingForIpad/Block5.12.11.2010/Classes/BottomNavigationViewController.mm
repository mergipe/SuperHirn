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
		ArticleItemSpacing = 194;

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
	self.view.backgroundColor = [UIColor greenColor];
}


- (void)addNavItem:(NSString*) iTitle
{
	
	int count = [navItems count];
	mScrollView.contentSize = CGSizeMake(ArticleItemSpacing * (count + 1), 300.0);
	
	int x = ArticleItemSpacing * count;
	BottomNavigationItemView *v = [[BottomNavigationItemView alloc] initWithFrame:CGRectMake(x, 0.0, ArticleItemSpacing, 300.0)];
	v.delegate = self;
	v.titleLabel.text = iTitle;
	//v.iconView.image = [UIImage imageWithContentsOfFile:iconPath];	
	
	
	[mScrollView addSubview:v];
	fScrollWidth = (x + ArticleItemSpacing);	
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
		xOffset = [navItems count] * ArticleItemSpacing - ArticleItemSpacing/2.0;
	}
	else
	{
		xOffset = ArticleItemSpacing * (iIndex + 1) - ArticleItemSpacing/2.0 - width/2.0;
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
