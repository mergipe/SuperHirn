//
//  ModuleInfoView.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "ModuleInfoView.h"


@implementation ModuleInfoView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		// add foreground image:
		
		picture = [[UIImageView alloc] initWithFrame:itemArea];
		[self addSubview:picture];
		
		// create a lable for the table header:
		text = [[[UILabel alloc] initWithFrame:CGRectMake(50, 50, 700, 500)] autorelease];
		text.text = @"My iCompetence Files";
		text.textColor = [UIColor whiteColor];
		text.shadowColor = [UIColor blackColor];
		text.textAlignment =  UITextAlignmentCenter;
		text.font = [UIFont boldSystemFontOfSize:22];
		text.backgroundColor = [UIColor clearColor];
		[self addSubview:text];
		
    }
    return self;
}

- (void) setText:(NSString*) iText
{
	self.text = iText;
}

- (void) setImage:(UIImage*) iImage
{
	
	picture.image = iImage;	
	itemArea.size = picture.image.size;

	// check that the picture isnt bigger than super view itself:
	
	if( itemArea.size.width > self.frame.size.width )
	{
		itemArea.size.width = self.frame.size.width;
	}
	if( itemArea.size.height > self.frame.size.height )
	{
		itemArea.size.height = self.frame.size.height;
	}
	
	// place the image in the center of the view
	itemArea.origin.x = 50;
	itemArea.origin.y = 50;	
	picture.frame = itemArea;	
	
	// adjust the label:
	text.frame.origin.y = picture.frame.origin.y + picture.frame.size.height + 50;
}


- (void)dealloc
{
	[picture release];
	[super dealloc];
}


@end
