	//
	//  ImageLegendView.mm
	//  AkiliPad
	//
	//  Created by Lukas Mueller on 21.6.2010
	//  Copyright 2010 Blankpage AG. All rights reserved.
	//

#import "ImageLegendView.h"
#import "AkiliPadEnvironment.h"


@implementation ImageLegendView


@synthesize textLegend;

- (id)initWithFrame:(CGRect)frame {

    if ((self = [super initWithFrame:frame])) {
		
		[self setBackgroundColor:[UIColor clearColor]];
		
		
		UIFont *font = [AkiliPadEnvironment textFont];
		
		CGPoint origin;
		origin.x = 0.0;
		origin.y = 0.0;
		
		CGPoint space;
		space.x = 10;
		space.y = 10;

		self->textDimension.x = 768;
		self->textDimension.y = space.y + font.pointSize * 6 + space.y;
		
		
		self->textLegend = [[UITextView alloc] 
					 initWithFrame:CGRectMake(
											  origin.x, origin.y, 
											  self->textDimension.x, self->textDimension.y )
					 ];
		
		self->textLegend.backgroundColor = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.65];
		self->textLegend.textColor = [UIColor whiteColor];
		self->textLegend.font = font;
		self->textLegend.textAlignment = UITextAlignmentCenter;
		
		[self setLegend:@""];
		
		[self addSubview:self->textLegend];
	
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

- (void) setLegend:(NSString*)iText
{
	self->textLegend.text = iText;
}

- (void) hide
{
	super.hidden = YES;
	
}

- (BOOL) isVisible
{
	if( super.hidden)
	{
		return NO;
	}
	else{
		return YES;
	}
}

- (void) show
{
	
	if( [self->textLegend.text length] > 0 )
	{
		super.hidden = NO;	
	}
}


- (CGPoint) size
{
	return self->textDimension;
}

- (void)dealloc {
	[textLegend release];
	[super dealloc];
}


@end
