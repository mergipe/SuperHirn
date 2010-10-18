//
//  ItemView.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "ItemView.h"
#import "StudentController.h"


@implementation ItemView

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		self.backgroundColor = [UIColor clearColor];
		itemArea.origin.x = 0;
		itemArea.origin.y = 0;
		itemArea.size.width = 0;
		itemArea.size.height = 0;
		
		// add an invisible border rectangle:
		borderRectangle = [[UIView alloc ] initWithFrame:  CGRectMake( 0,0,0,0 )];
		[ self addSubview:borderRectangle ];
		
    }
    return self;
}

- (void) addBorder
{
	int borderSize = 10;
	UIColor* color = [UIColor blueColor];
	
	CGRect bgArea = CGRectMake( itemArea.origin.x - borderSize, itemArea.origin.y - borderSize , 
							   itemArea.size.width + 2 * borderSize, itemArea.size.height + 2 * borderSize );
	borderRectangle.frame = bgArea;
	borderRectangle.backgroundColor = color;
}

- (void) addBorder:(CGFloat)iSize andColor:(UIColor*)iColor
{
	CGRect bgArea = CGRectMake( itemArea.origin.x - iSize, itemArea.origin.y - iSize , 
							   itemArea.size.width + 2*iSize, itemArea.size.height + 2*iSize );
	borderRectangle.frame = bgArea;
	borderRectangle.backgroundColor = iColor;
}


- (void)setItemIndex:(int)iIndex
{
	itemIndex = iIndex;
}


- (void)dealloc
{
	[borderRectangle release];
	[super dealloc];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject]; //assume just 1 touch
	if(touch.tapCount == 1) 
	{
		NSLog(@"Tapped on item at %d", itemIndex);
		NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:itemIndex] forKey:@"itemIndex"];
		[[NSNotificationCenter defaultCenter] postNotificationName:@"showFullScreenItemView" object:self userInfo:dict];
	}
}

@end
