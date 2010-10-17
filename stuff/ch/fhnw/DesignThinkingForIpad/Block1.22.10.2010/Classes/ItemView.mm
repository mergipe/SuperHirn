#import "ItemView.h"


#import "StudentController.h"

@implementation ItemView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		        
		self.backgroundColor = [UIColor clearColor];
		itemArea.size.width = 0;
		itemArea.size.height = 0;
		itemArea.origin.x = 0;
		itemArea.origin.y = 0;
		fullScreenMode = NO;
		

		CGRect rect = CGRectMake(10 , 10, 120.0f, 45.0f);
		title.textColor = [UIColor whiteColor];
		self.title = [[[UILabel alloc] initWithFrame:rect] autorelease];
		[title setTextAlignment:UITextAlignmentCenter];
		[title setText:@"TTTEEEST"];
		title.font = [UIFont boldSystemFontOfSize:30];


		// add an invisible border rectangle:
		borderRectangle = [[UIView alloc ] initWithFrame:  CGRectMake( 0,0,0,0 )];
		[ self addSubview:borderRectangle ];
		
		
		/*
		title.frame.origin.x = 0;
		title.frame.origin.y = 100;
		title.frame.size.width = 100;
		title.frame.size.height = 45;
		 */
		[self addSubview:title];
		
    }
    return self;
}

- (void) addBorder:(int)iSize andColor:(UIColor*)iColor
{
	CGRect bgArea = CGRectMake( itemArea.origin.x - iSize, itemArea.origin.y - iSize , 
							   itemArea.size.width + 2*iSize, itemArea.size.height + 2*iSize );
	borderRectangle.frame = bgArea;
	borderRectangle.backgroundColor = iColor;
}


- (void) setFullScreen:(BOOL) iFullScreen
{
	fullScreenMode = iFullScreen;
}

- (void) setTitle:(NSString*) iTitle
{
	[title setText:@"where is my text"];

}


- (void)setItemIndex:(int)iIndex
{
	itemIndex = iIndex;
}


- (void)dealloc
{
	[title release];
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
