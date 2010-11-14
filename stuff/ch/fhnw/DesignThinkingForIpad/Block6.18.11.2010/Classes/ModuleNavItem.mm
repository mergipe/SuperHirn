


#import "ModuleNavItem.h"


@implementation ModuleNavItem

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) {
		        
		self.backgroundColor = [UIColor clearColor];
		
		CGRect f = CGRectMake(0.0, 0.0, 100.0, 100.0);
		self.backgroundIconImageView.frame = f; 
		
		f = CGRectMake(0.0, 100.0, self.frame.size.width, 20.0);
		titleLabel = [[UILabel alloc] initWithFrame:f];
		titleLabel.backgroundColor = [UIColor clearColor];
		titleLabel.textColor = [UIColor blackColor];
		titleLabel.numberOfLines = 0;
		titleLabel.textAlignment = UITextAlignmentCenter;
		
		[self addSubview:titleLabel];
		
    }
    return self;
}



@end
