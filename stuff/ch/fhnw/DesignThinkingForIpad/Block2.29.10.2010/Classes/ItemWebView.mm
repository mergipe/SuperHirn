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
		[self addSubview:webItem];
	}
	
	// check that the webview isnt bigger than super view itself:
	if( itemArea.size.width > self.frame.size.width )
	{
		itemArea.size.width = self.frame.size.width;
	}
	if( itemArea.size.height > self.frame.size.height )
	{
		itemArea.size.height = self.frame.size.height;
	}
	
	// place the image in the center of the view
	itemArea.origin.x = (self.frame.size.width - itemArea.size.width) / 2;
	itemArea.origin.y = (self.frame.size.height - itemArea.size.height) / 2;
		
	webItem.frame = itemArea;
	
	
	// load the ur:
	NSURL *url = [NSURL fileURLWithPath:path];
	NSURLRequest *request = [NSURLRequest requestWithURL:url];
	[webItem loadRequest:request];

}


- (void)dealloc
{
	[webItem release];
	[super dealloc];
}


@end
