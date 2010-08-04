//
//  BottomNavigationItemView.h
//  AkiliPad
//
//  Created by Mithin on 04/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>


@protocol BottomNavItemDelegate <NSObject>

@optional
- (void)tappedOnItem:(UIView *)view;

@end

@class AkiliLabel;

@interface BottomNavigationItemView : UIView {
	UIImageView *iconView;
	AkiliLabel *titleLabel;
	//UILabel *subTitleLabel;
	
	id <BottomNavItemDelegate> delegate;
	
	
@private
		// coordinates and size of icon / text
	CGPoint iconPosition;
	CGPoint iconSize;
	CGPoint titlePosition;
	CGPoint titleSize;

		// coordinates and size of icon / text
	float verticalSpacer;
	
	
}

@property(nonatomic, retain) UIImageView *iconView;
@property(nonatomic, retain) AkiliLabel *titleLabel;
//@property(nonatomic, retain) UILabel *subTitleLabel;
@property(nonatomic, assign) id<BottomNavItemDelegate> delegate;


@end
