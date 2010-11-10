//
//  BottomNavigationItemView.mm
//  AkiliPad
//
//  Created by Mithin on 04/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "BottomNavigationItemView.h"


@implementation BottomNavigationItemView

@synthesize iconView;
@synthesize titleLabel;
@synthesize delegate;
@synthesize backgroundIconImageView;

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) {
		        
		self.backgroundColor = [UIColor clearColor];
		
		CGRect f = CGRectMake(7.0, 21.0, 180.0, 180.0);
		iconView = [[UIImageView alloc] initWithFrame:f];
		
		CGRect fBG = CGRectMake(0.0, 0.0, 180.0, 180.0);
		backgroundIconImageView = [[UIImageView alloc] initWithFrame:fBG];
		backgroundIconImageView.image = [UIImage imageNamed:@"file-icon.png"];
		[backgroundIconImageView addSubview:iconView];
		[self addSubview:backgroundIconImageView];
		
		f = CGRectMake(0.0, 200.0, self.frame.size.width, 20.0);
		titleLabel = [[UILabel alloc] initWithFrame:f];
		titleLabel.backgroundColor = [UIColor clearColor];
		titleLabel.textColor = [UIColor blackColor];
		titleLabel.numberOfLines = 0;
		titleLabel.textAlignment = UITextAlignmentCenter;
		
		[self addSubview:titleLabel];

    }
    return self;
}


- (void)dealloc {
	[iconView release];
	[titleLabel release];
	[super dealloc];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
	UITouch *touch = [touches anyObject]; //assume just 1 touch
	if(touch.tapCount == 1) {
		if(delegate && [delegate respondsToSelector:@selector(tappedOnItem:)]) 
		{
			[delegate tappedOnItem:self];
		}
	}
}

@end
