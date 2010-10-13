#import "PictureView.h"
#import "StudentController.h"


@implementation PictureView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		// initialize all parameters:
		scaleFactor = 1.0;
	
		// add foreground image:
		picture = [[UIImageView alloc] initWithFrame:itemArea];
		[self addSubview:picture];
    }
    return self;
}





- (void) setImage:(NSString*) iImageName
{
	
	NSString* picturePath  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iImageName];
	picture.image = [UIImage imageWithContentsOfFile:picturePath];	
	itemArea.size = picture.image.size;

	
	if( itemArea.size.width > self.frame.size.width )
	{
		itemArea.size.width = self.frame.size.width;
	}
	if( itemArea.size.height > self.frame.size.height )
	{
		itemArea.size.height = self.frame.size.height;
	}
	
	if( fullScreenMode) 
	{
		scaleFactor = self.frame.size.width / itemArea.size.width;
	}
	else 
	{
		scaleFactor = 1;
	}

	itemArea.size.width *= scaleFactor;
	itemArea.size.height *= scaleFactor;
	
	itemArea.origin.x = (self.frame.size.width - itemArea.size.width) / 2;
	itemArea.origin.y = (self.frame.size.height - itemArea.size.height) / 2;
	
	picture.frame = itemArea;

	[self setTitle:iImageName];
	
}


- (void)dealloc
{
	[picture release];
	[super dealloc];
}


@end
