#import "ItemView.h"


#import "StudentController.h"
#import <QuartzCore/QuartzCore.h>

@implementation ItemView

@synthesize delegate;

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		        
		/*
		CAGradientLayer *gradient = [CAGradientLayer layer];
		gradient.frame = self.frame;
		UIColor *startColor = [UIColor colorWithWhite: 0.5 alpha: 1.0];
		UIColor *endColor = [UIColor blackColor];
		gradient.colors = [NSArray arrayWithObjects:(id)[startColor CGColor], (id) [endColor CGColor], nil];
		[self.layer insertSublayer:gradient atIndex:1];
		*/
		self.backgroundColor = [UIColor clearColor];
		imageArea.size.width = 0;
		imageArea.size.height = 0;
		imageArea.origin.x = 0;
		imageArea.origin.y = 0;
			
		// add background image:
		background = [[UIImageView alloc] initWithFrame:imageArea];
		background.image = [UIImage imageNamed:@"shadow.png"];
		[self addSubview:background];
		
		// add foreground image:
		picture = [[UIImageView alloc] initWithFrame:imageArea];
		[self addSubview:picture];
	
		
    }
    return self;
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


- (void) setImage:(NSString*) iImageName
{
	
	NSString* picturePath  = [[StudentController getPictureFolderPath] stringByAppendingPathComponent:iImageName];
	picture.image = [UIImage imageWithContentsOfFile:picturePath];	
	imageArea.size = picture.image.size;

	[self setTitle:iImageName];
	
	if( imageArea.size.width > self.frame.size.width )
	{
		imageArea.size.width = self.frame.size.width;
	}
	if( imageArea.size.height > self.frame.size.height )
	{
		imageArea.size.height = self.frame.size.height;
	}
	
	
	imageArea.origin.x = (self.frame.size.width - imageArea.size.width) / 2;
	imageArea.origin.y = (self.frame.size.height - imageArea.size.height) / 2;
	
	picture.frame = imageArea;
	
	int offset = 0;
	CGRect bgArea = imageArea;
	bgArea.size.width += 2 * offset;
	bgArea.size.height += 2 * offset;
	bgArea.origin.x -= offset;
	bgArea.origin.y -= offset;
	background.frame = bgArea;
}


- (void)dealloc
{
	[picture release];
	[title release];
	[background release];
	[super dealloc];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject]; //assume just 1 touch
	if(touch.tapCount == 1) 
	{
		NSLog(@"Tapped on item at %d", itemIndex);
		NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:itemIndex] forKey:@"imageIndex"];
		[[NSNotificationCenter defaultCenter] postNotificationName:@"showFullScreenPictureView" object:self userInfo:dict];
/*		
		//single tap occurred
		if(delegate && [delegate respondsToSelector:@selector(tappedOnItem:)]) {
			[delegate tappedOnItem:itemIndex];
		}
 */
	}
}

@end
