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
		
    }
    return self;
}


- (void) setFullScreen:(BOOL) iFullScreen
{
	fullScreenMode = iFullScreen;
}

- (void) setTitle:(NSString*) iTitle
{
	title.frame.origin.x = 0;
	title.frame.origin.y = 100;
	title.frame.size.width = 100;
	title.frame.size.height = 45;
	title.text = @"where is my text";

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
