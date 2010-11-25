//
//  ItemView.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "PictureView.h"
#import "ContactDetail.h"
#import "QuartzCore/QuartzCore.h"

@implementation ContactDetail

@synthesize emailDelegate;

- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		// create a new subview for the popover of the table dark:
		UIView *containerView2 = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 770, 960)] autorelease];
		containerView2.backgroundColor = [UIColor colorWithWhite:0.000 alpha:0.600];
		[ self addSubview:containerView2];
		
		self.backgroundColor = [UIColor clearColor];
		
		/*
		 // create a new subview for the popover2 of the table:
		 UIView *containerView4 = [[[UIView alloc] initWithFrame:CGRectMake(600, 95, 20, 80)] autorelease];
		 containerView4.backgroundColor = [UIColor colorWithWhite:1.000 alpha:1.000];
		 [ self addSubview:containerView4];
		 */
		// create a new subview for the popover2 of the table bright:
		UIView *containerView3 = [[[UIView alloc] initWithFrame:CGRectMake(80, 100, 600, 800)] autorelease];
		containerView3.backgroundColor = [UIColor colorWithWhite:1.000 alpha:0.800];
		//--shadow----
		containerView3.layer.shadowColor = [[UIColor blackColor]CGColor];
		containerView3.layer.shadowOffset = CGSizeMake(10.0f,10.0f);
		containerView3.layer.shadowOpacity = 1.0f;
		containerView3.layer.shadowRadius = 10.0f;
		//containerView3.layer.edgeAntialiasingMask = 1000;
		containerView3.layer.cornerRadius = 10;			// Ecken abrunden
		
		UILabel *contactName = [[[UILabel alloc] initWithFrame:CGRectMake(10, 20, 480, 100)] autorelease];
		contactName.text = @"Max Haueter";
		contactName.textColor = [UIColor blackColor];
		contactName.font = [UIFont systemFontOfSize:64];
		contactName.backgroundColor = [UIColor clearColor];
		[containerView3 addSubview:contactName];
		[self addSubview:containerView3];
		
		UIImageView* person = [[UIImageView alloc] initWithFrame:CGRectMake(200, 300, 360, 360)];
		person.image = [UIImage imageNamed:@"person2.gif"];
		[ self addSubview:person];
		
		
		//Send e-mail button----------------
		PictureView* emailPicNew = [[PictureView alloc] initWithFrame:CGRectMake(550, 780, 100, 100)];
		[emailPicNew setResourceImage:@"sendEmail.png"];
		emailPicNew.delegate = self;
		[ self addSubview:emailPicNew];
		

		//----------------------------------
		
		
		
    }
    return self;
}


//-(id)initWithImage:(UIImage *)image;

/*-------- event für email senden
 -(UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event;
 -(CGPoint)locationInView:(UIView *)view;
 
 -(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
 CGPoint locationPoint = [[touches anyObject] locationInView:self];
 //	UIView* viewsdjalkljals = [self hitTest:locationPoint withEvent:event];
 NSLog(@" Contact will closed" );
 self.hidden=NO;
 }
 ------------ */


-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSLog(@" Contact will closed" );
	self.hidden=YES;
}

-(void)tapOnItem:(int)iItemIndex;
{
	NSLog(@"Send email requested");
	[emailDelegate sendEmail];
}

@end
