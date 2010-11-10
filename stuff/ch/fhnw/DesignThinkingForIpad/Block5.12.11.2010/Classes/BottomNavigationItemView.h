//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>


@protocol BottomNavItemDelegate <NSObject>

@optional
- (void)tappedOnItem:(UIView *)view;

@end

@interface BottomNavigationItemView : UIView 
{
	UIImageView *backgroundIconImageView;
	UILabel* titleLabel;
	id <BottomNavItemDelegate> delegate;
}

@property(nonatomic, retain) UIImageView *iconView;
@property(nonatomic, retain) UILabel *titleLabel;
@property(nonatomic, retain) UIImageView *backgroundIconImageView;
@property(nonatomic, assign) id<BottomNavItemDelegate> delegate;


@end
