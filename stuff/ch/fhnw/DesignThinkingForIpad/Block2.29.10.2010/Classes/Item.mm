//
//  ItemView.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "Item.h"
#import "StudentController.h"


@implementation Item

@synthesize delegate;

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



- (void) addBorder:(CGFloat)iSize :(UIColor*)iColor
{
	CGRect bgArea = CGRectMake( itemArea.origin.x - iSize, itemArea.origin.y - iSize , 
							   itemArea.size.width + 2*iSize, itemArea.size.height + 2*iSize );
	borderRectangle.frame = bgArea;
	borderRectangle.backgroundColor = iColor;
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{

	NSLog(@" Tap on item: %d", itemIndex );

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


@end
