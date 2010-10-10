
#import "ItemPageView.h"
#import "ItemView.h"

#import "StudentController.h"


@implementation ItemPageView




- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		iconHeight = 200;
		iconWidth = 200;
		spacing = 50;
		self.backgroundColor = [UIColor clearColor];
	}
    return self;
}

+(int) numberItemsPerPage
{
	return 4;
}

- (void)setItemIndexes:(int) iStartIndex withEndIndex:(int) iEndIndex
{
	
	CGRect f = CGRectMake(spacing, spacing, iconWidth, iconHeight);
	int colCount = 0;
	while( iStartIndex < iEndIndex + 1)
	{
		if( iStartIndex >= [StudentController numberOfPictures] )
		{
			break;
		}
		
		NSString* itemName = [StudentController getPicture:iStartIndex];
		ItemView* item = [[ItemView alloc] initWithFrame:f];
		//item.delegate = self;		
		[ item setItemIndex:iStartIndex];
		[item  setImage:itemName];
		[self addSubview:item];
		
		if( colCount == 2 )
		{
			colCount = 0;
			f.origin.x = spacing;
			f.origin.y += spacing + iconHeight;
		}
		else
		{
			f.origin.x += iconWidth + spacing;
			colCount++;
		}

		iStartIndex++;
	}
	
}

- (void)dealloc
{
	[super dealloc];
}

@end
