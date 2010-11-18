//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "ModuleBottomNavController.h"
#import "ModuleNavItem.h"


@implementation ModuleBottomNavController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
		ItemSpacing = 120;
		ScrollViewHeight = 80;

    }
    return self;
}


- (void)addNavItem:(NSString*) iTitle: (NSString*) iconPath
{
	
	int count = [navItems count];
	mScrollView.contentSize = CGSizeMake(ItemSpacing * (count + 1), ScrollViewHeight);
	
	int x = ItemSpacing * count;
	ModuleNavItem *v = [[ModuleNavItem alloc] initWithFrame:CGRectMake(x, 0.0, ItemSpacing, ScrollViewHeight)];
	v.delegate = self;
	v.titleLabel.text = iTitle;
	v.backgroundIconImageView.image = [UIImage imageNamed:iconPath];	
	
	
	[mScrollView addSubview:v];
	fScrollWidth = (x + ItemSpacing);	
	[navItems addObject:v];
	[v release];
}



@end
