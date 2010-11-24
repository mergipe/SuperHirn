//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <QuartzCore/QuartzCore.h>

#import "BottomNavigationItemView.h"
#import "StudentController.h"

@implementation BottomNavigationItemView

@synthesize titleLabel;
@synthesize delegate;
@synthesize backgroundIconImageView;

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) {
		        
		self.backgroundColor = [UIColor clearColor];
		
		CGRect f = CGRectMake(0.0, 0.0, 180.0, 180.0);
		backgroundIconImageView = [[UIImageView alloc] initWithFrame:f];
		backgroundIconImageView.image = [UIImage imageNamed:@"file-icon.png"];
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

- (id)initWithFrameX:(CGRect)frame:(int)i
{
    if ((self = [super initWithFrame:frame])) {
		
		self.backgroundColor = [UIColor clearColor];
		
		CGRect f = CGRectMake(0.0, 0.0, 180.0, 180.0);
		backgroundIconImageView = [[UIImageView alloc] initWithFrame:f];
		backgroundIconImageView.image = [UIImage imageWithContentsOfFile: [[StudentController getImageDataFolderPath] stringByAppendingPathComponent:[StudentController getFile:i]]];
		[self addSubview:backgroundIconImageView];
		
		f = CGRectMake(0.0, 200.0, self.frame.size.width, 20.0);
		titleLabel = [[UILabel alloc] initWithFrame:f];
		titleLabel.font = [UIFont boldSystemFontOfSize:22];
		titleLabel.backgroundColor = [UIColor clearColor];
		titleLabel.textColor = [UIColor whiteColor];
		titleLabel.numberOfLines = 0;
		titleLabel.textAlignment = UITextAlignmentCenter;
		
		[self addSubview:titleLabel];
		
    }
    return self;
}
- (void)dealloc {
	[backgroundIconImageView release];
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
