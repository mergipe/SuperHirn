#import "ItemWebView.h"


#import "StudentController.h"

@implementation ItemWebView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		        
		self.backgroundColor = [UIColor clearColor];
		webArea.size.width = 768;
		webArea.size.height = 1024;
		webArea.origin.x = 0;
		webArea.origin.y = 0;
					
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


- (void) setUrl:(NSString*) iUrl
{
	
	NSString* path  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iUrl];
	
	if( webItem == nil )
	{
		webItem = [[UIWebView alloc] init];
		//webItem.delegate = self;
		webItem.backgroundColor = [UIColor clearColor];
		webItem.frame = webArea;
		[self addSubview:webItem];
	}
	
	NSURL *url = [NSURL fileURLWithPath:path];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webItem loadRequest:request];
	[self setTitle:iUrl];
	
}


- (void)dealloc
{
	[webItem release];
	[title release];
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
	}
}

@end
