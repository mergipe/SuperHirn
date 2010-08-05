//
//  OverviewItemView.h
//  iCompetence
//
//  Authors: SH/LM on 07/06/10.
//  
//

#import <UIKit/UIKit.h>

@protocol OverviewItemDelegate <NSObject>

@optional
- (void)tappedOnItem:(UIView *)view;

@end


@interface OverviewItemView : UIView {
	UIImageView *thumbnailView;
	UITextView *titleLabel;
	UILabel *issueLabel;
	UILabel *subTitleLabel;
		
	id <OverviewItemDelegate> delegate;
}

@property(nonatomic, retain) UIImageView *thumbnailView;
@property(nonatomic, retain) UITextView *titleLabel;
@property(nonatomic, retain) UILabel *issueLabel;
@property(nonatomic, retain) UILabel *subTitleLabel;
@property(nonatomic, assign) id<OverviewItemDelegate> delegate;

@end
