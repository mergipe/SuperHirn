	//
	//  LibraryItemView.mm
	//  AkiliPad
	//
	//  Created by Mithin on 07/06/10.
	//  Copyright 2010 Techtinium Corporation. All rights reserved.
	//

#import "LibraryItemView.h"


@implementation LibraryItemView

@synthesize thumbnailView;
@synthesize issueLabel;

@synthesize titleLabel;
@synthesize subTitleLabel;
@synthesize delegate;

- (id)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
		
		[self setBackgroundColor:[UIColor clearColor]];
		
		CGPoint origin;
		origin.x = 5.0;
		origin.y = 5.0;
		
		CGPoint thumbSize;
		thumbSize.x = 150;
		thumbSize.y = 200;
		
		CGPoint space;
		space.x = 10;
		space.y = 10;
		
		
		//UIFont *textFont = [AkiliPadEnvironment textFont];
		
		
			// set thumbnail size:
		thumbnailView = [[UIImageView alloc] initWithFrame:CGRectMake(origin.x, origin.y, thumbSize.x, thumbSize.y)];
		
		
			// single line issue number:
			//issueLabel = [[UILabel alloc] initWithFrame:CGRectMake(origin.x, origin.y + thumbSize.y + space.y, thumbSize.x, textFont.pointSize )];
		issueLabel.backgroundColor = [UIColor clearColor];
		issueLabel.numberOfLines = 1;
		issueLabel.textColor = [UIColor whiteColor];
		//issueLabel.font = [AkiliPadEnvironment textFont];
		issueLabel.textAlignment = UITextAlignmentCenter;
		
			// multi line issue title:
			//titleLabel = [[UITextView alloc] initWithFrame:CGRectMake(origin.x, origin.y + thumbSize.y + space.y + textFont.pointSize, thumbSize.x, textFont.pointSize * 3 )];
		titleLabel.backgroundColor = [UIColor clearColor];
			//titleLabel.numberOfLines = 0;
		titleLabel.textColor = [UIColor whiteColor];
		//titleLabel.font = [AkiliPadEnvironment textFont];
		titleLabel.textAlignment = UITextAlignmentCenter;
		
		subTitleLabel = [[UILabel alloc] initWithFrame:CGRectMake(5.0, 240.0, 150.0, 50.0)];
		subTitleLabel.backgroundColor = [UIColor clearColor];
		subTitleLabel.numberOfLines = 0;
		subTitleLabel.textColor = [UIColor grayColor];
		subTitleLabel.textAlignment = UITextAlignmentCenter;
		//subTitleLabel.font = [AkiliPadEnvironment subtitleFont];
		
		[self addSubview:thumbnailView];
		[self addSubview:issueLabel];
		[self addSubview:titleLabel];
			//[self addSubview:subTitleLabel];
    }
    return self;
}


	// Only override drawRect: if you perform custom drawing.
	// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
		// Drawing a gray line as separator
	/*
	CGContextRef ctx = UIGraphicsGetCurrentContext(); //get the graphics context
	 CGContextSetRGBStrokeColor(ctx, 100.0/255.0, 100.0/255.0, 100.0/255.0, 1);
	 CGContextMoveToPoint(ctx, self.frame.size.width - 5, 10.0);
	 CGContextAddLineToPoint(ctx, self.frame.size.width - 5, self.frame.size.height - 10.0);
	 CGContextStrokePath(ctx);
	 */
}

- (void)dealloc {
	[thumbnailView release];
	[titleLabel release];
	[issueLabel release];
	[subTitleLabel release];
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
