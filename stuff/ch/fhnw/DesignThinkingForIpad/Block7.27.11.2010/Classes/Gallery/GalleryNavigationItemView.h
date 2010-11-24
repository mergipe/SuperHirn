//
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>


@protocol GalleryNavigationItemViewDelegate <NSObject>

@optional
- (void)tappedOnItem:(UIView *)view;

@end

@interface GalleryNavigationItemView : UIView 
{
	UIImageView *backgroundIconImageView;
	UILabel* titleLabel;
	id <GalleryNavigationItemViewDelegate> delegate;
}

@property(nonatomic, retain) UILabel *titleLabel;
@property(nonatomic, retain) UIImageView *backgroundIconImageView;
@property(nonatomic, assign) id<GalleryNavigationItemViewDelegate> delegate;

// -(void)initWithFrameX:(UIView*)iView ItemIndex:(int)i; // alt!!
-(id)initWithFrameX:(CGRect)frame :(int)i;

@end
