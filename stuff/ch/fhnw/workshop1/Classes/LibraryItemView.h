//
//  LibraryItemView.h
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol LibraryItemDelegate <NSObject>

@optional
- (void)tappedOnItem:(UIView *)view;

@end


@interface LibraryItemView : UIView {
	UIImageView *thumbnailView;
	UITextView *titleLabel;
	UILabel *issueLabel;
	UILabel *subTitleLabel;
		
	id <LibraryItemDelegate> delegate;
}

@property(nonatomic, retain) UIImageView *thumbnailView;
@property(nonatomic, retain) UITextView *titleLabel;
@property(nonatomic, retain) UILabel *issueLabel;
@property(nonatomic, retain) UILabel *subTitleLabel;
@property(nonatomic, assign) id<LibraryItemDelegate> delegate;

@end
