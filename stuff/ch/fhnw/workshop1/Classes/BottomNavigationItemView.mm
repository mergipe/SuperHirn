//
//  BottomNavigationItemView.mm
//  AkiliPad
//
//  Created by Mithin on 04/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "BottomNavigationItemView.h"
#import "AkiliLabel.h"
#import "AkiliPadEnvironment.h"

@implementation BottomNavigationItemView

@synthesize iconView;
@synthesize titleLabel;
//@synthesize subTitleLabel;
@synthesize delegate;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		        
		// Initialization positions
		verticalSpacer = 10;
		
		iconPosition.x = 5.0;
		iconPosition.y = 10.0;
		iconSize.x = 100.0;
		iconSize.y = 150.0;
		
		titlePosition.x = iconSize.x + verticalSpacer;
		titlePosition.y = iconPosition.y + iconSize.x / 2.0 - 10;
		titleSize.x = 100.0;
		titleSize.y = iconSize.y;
		
		
		iconView = [[UIImageView alloc] 
					initWithFrame:CGRectMake(
											 iconPosition.x, iconPosition.y, 
											 iconSize.x, iconSize.y
											 )];
		
		titleLabel = [[AkiliLabel alloc] 
					  initWithFrame:CGRectMake(
											   titlePosition.x, titlePosition.y, 
											   titleSize.x, titleSize.y 
											   )];
		
		titleLabel.backgroundColor = [UIColor clearColor];
		titleLabel.textColor = [UIColor whiteColor];
		
			// Lukas: add customized fonts:
		titleLabel.font = AkiliPadEnvironment.textFont;
			//titleLabel.font = [UIFont fontWithName: @"Black-Medium" size: 17];
			//titleLabel.font = [UIFont boldSystemFontOfSize:17.0];
		titleLabel.numberOfLines = 0;

		
		//subTitleLabel = [[UILabel alloc] initWithFrame:CGRectMake(85.0, 55.0, 125.0, 90.0)];
		//subTitleLabel.backgroundColor = [UIColor clearColor];
		//subTitleLabel.numberOfLines = 0;
		//subTitleLabel.textColor = [UIColor grayColor];
		//subTitleLabel.font = [UIFont systemFontOfSize:12.0];
		
		[self addSubview:iconView];
		[self addSubview:titleLabel];
		//[self addSubview:subTitleLabel];
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
	// Drawing a gray line as separator
	CGContextRef ctx = UIGraphicsGetCurrentContext(); //get the graphics context
	CGContextSetRGBStrokeColor(ctx, 100.0/255.0, 100.0/255.0, 100.0/255.0, 1);
	CGContextMoveToPoint(ctx, self.frame.size.width - 5, 10.0);
	CGContextAddLineToPoint(ctx, self.frame.size.width - 5, self.frame.size.height - 10.0);
	CGContextStrokePath(ctx);
}

- (void)dealloc {
	[iconView release];
	[titleLabel release];
	//[subTitleLabel release];
	[super dealloc];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	UITouch *touch = [touches anyObject]; //assume just 1 touch
	if(touch.tapCount == 1) {
		//single tap occurred
		if(delegate && [delegate respondsToSelector:@selector(tappedOnItem:)]) {
			[delegate tappedOnItem:self];
		}
	}
}

@end
