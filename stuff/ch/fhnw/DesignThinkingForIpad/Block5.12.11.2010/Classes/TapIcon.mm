//
//  TapIcon.mm
//  iPad Block 1-6 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "TapIcon.h"


@implementation TapIcon

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		
		//[self addBorder:1 : [UIColor greenColor] ];
		
    }
    return self;
}


-(void)setTitle:(NSString*)iTitle
{
	CGRect rect = CGRectMake(0,0, self.frame.size.width, 20);
	UILabel* label = [[[UILabel alloc] initWithFrame:rect] autorelease];
	[label setText:iTitle];
	[label setTextAlignment:UITextAlignmentCenter];
	[self addSubview:label];
}


-(void)setIcon:(UIImage*)iImage
{
	CGRect rect = CGRectMake(0,0, self.frame.size.width, self.frame.size.height);
	UIImageView* picture = [[UIImageView alloc] initWithFrame:rect];
	picture.image = iImage;
	[self addSubview:picture];

}

@end
