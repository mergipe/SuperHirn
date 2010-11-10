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

@interface BottomNavigationItemView : UIView {
	UIImageView *iconView;
	UIImageView *backgroundIconImageView;
	
	id <BottomNavItemDelegate> delegate;
}

@property(nonatomic, retain) UIImageView *iconView;
@property(nonatomic, retain) UILabel *titleLabel;
@property(nonatomic, retain) UIImageView *backgroundIconImageView;
@property(nonatomic, assign) id<BottomNavItemDelegate> delegate;


@end
