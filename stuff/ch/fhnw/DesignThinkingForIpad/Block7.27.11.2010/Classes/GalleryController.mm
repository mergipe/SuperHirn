
#import "GalleryController.h"
#import <QuartzCore/QuartzCore.h>

@implementation GalleryController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
					
    }
    return self;
}


-(void)addSwipeItemX:(UIView*)iView ItemIndex:(int)i
{
	if( allSwipeItems == nil )
	{
		allSwipeItems = [[NSMutableArray alloc] init];
	}	
	[allSwipeItems addObject:iView];
	
	[bottomNavViewController addNavItemX:[StudentController getFile:i ] :i];
	
}

@end
