//
//  ItemView.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "ContactDetail.h"


@implementation ContactDetail


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		// create a new subview for the popover of the table:
		UIView *containerView2 = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 770, 960)] autorelease];
		containerView2.backgroundColor = [UIColor colorWithWhite:0.000 alpha:0.600];
		[ self addSubview:containerView2];
		
		self.backgroundColor = [UIColor clearColor];
		
		/*
		// create a new subview for the popover2 of the table:
		UIView *containerView4 = [[[UIView alloc] initWithFrame:CGRectMake(600, 95, 20, 80)] autorelease];
		containerView4.backgroundColor = [UIColor colorWithWhite:1.000 alpha:1.000];
		[ self addSubview:containerView4];
		*/
		// create a new subview for the popover2 of the table:
		UIView *containerView3 = [[[UIView alloc] initWithFrame:CGRectMake(80, 100, 600, 800)] autorelease];
		containerView3.backgroundColor = [UIColor colorWithWhite:1.000 alpha:0.800];
		
		UILabel *contactName = [[[UILabel alloc] initWithFrame:CGRectMake(10, 20, 480, 100)] autorelease];
		contactName.text = @"Max Max";
		contactName.textColor = [UIColor blackColor];
		contactName.font = [UIFont systemFontOfSize:64];
		contactName.backgroundColor = [UIColor clearColor];
		[containerView3 addSubview:contactName];
		[self addSubview:containerView3];
		
		
    }
    return self;
}




-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{

	NSLog(@" Contact will closed" );
	self.hidden=YES;
}


@end
