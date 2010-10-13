#import "ItemWebView.h"


#import "StudentController.h"

@implementation ItemWebView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		itemArea.size.width = 768;
		itemArea.size.height = 1024;
		itemArea.origin.x = 0;
		itemArea.origin.y = 0;
					
    }
    return self;
}




- (void) setUrl:(NSString*) iUrl
{
	
	NSString* path  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iUrl];
	
	if( webItem == nil )
	{
		webItem = [[UIWebView alloc] init];
		//webItem.delegate = self;
		webItem.backgroundColor = [UIColor clearColor];
		webItem.frame = itemArea;
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
	[super dealloc];
}


@end
