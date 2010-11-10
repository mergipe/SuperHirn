#import "ItemWebView.h"

#import "StudentController.h"

@implementation ItemWebView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{					
    }
    return self;
}




- (void) setUrl:(NSString*) iUrl
{
	
	NSString* path  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iUrl];
	
	if( webItem == nil )
	{
		webItem = [[UIWebView alloc] init];
		webItem.backgroundColor = [UIColor clearColor];
		webItem.frame = self.frame;
		[self addSubview:webItem];
	}
	
	
	NSString* ext = [iUrl pathExtension];
	if( [ext isEqualToString:@"pdf"] )
	{
		NSData* data = [NSData dataWithContentsOfFile:path];
		[webItem loadData:data MIMEType:@"application/pdf" textEncodingName:@"UTF-8" baseURL:nil];
	}
	else
	{
		NSURL *url = [NSURL fileURLWithPath:path];
		NSURLRequest *request = [NSURLRequest requestWithURL:url];
		[webItem loadRequest:request];
	}
}


- (void)dealloc
{
	[webItem release];
	[super dealloc];
}


@end
