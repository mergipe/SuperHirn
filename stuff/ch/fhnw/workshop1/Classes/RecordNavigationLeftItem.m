//
//  RecordNavigationLeftItem.m
//  iCompetence
//
//  Authors: SH/LM on 18/05/10.
//  
//

#import "RecordNavigationLeftItem.h"


@implementation RecordNavigationLeftItem

- (id)initForViewController:(UIViewController *)viewController {
	if(self == [super init]) {
		self.frame = CGRectMake(0.0, 0.0, 140.0, 50.0);
		self.backgroundColor = [UIColor clearColor];
		
		UIButton *backButton = [UIButton buttonWithType:UIButtonTypeCustom];
		backButton.frame = CGRectMake(15.0, 10.0, 63.0, 30.0);
		[backButton setImage:[UIImage imageNamed:@"btn_back.png"] forState:UIControlStateNormal];
		[backButton setImage:[UIImage imageNamed:@"btn_back.png"] forState:UIControlStateHighlighted];
		[self addSubview:backButton];
		backButton.tag = 1;
		[backButton addTarget:viewController action:@selector(goBack:) forControlEvents:UIControlEventTouchUpInside];
		
		UIButton *tocButton = [UIButton buttonWithType:UIButtonTypeCustom];
		tocButton.frame = CGRectMake(83.0, 10.0, 39.0, 30.0);
		[tocButton setImage:[UIImage imageNamed:@"btn_toc.png"] forState:UIControlStateNormal];
		[tocButton setImage:[UIImage imageNamed:@"btn_toc.png"] forState:UIControlStateHighlighted];
		[self addSubview:tocButton];
		tocButton.tag = 2;
		[tocButton addTarget:viewController action:@selector(showTOC:) forControlEvents:UIControlEventTouchUpInside];
		tocButton.hidden = YES;
	}
	return self;
}

@end
