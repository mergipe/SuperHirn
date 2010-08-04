//
//  ArticleSelectorViewController.h
//  AkiliPad
//
//  Created by Mithin on 18/05/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol ArticleSelectorDelegate

- (void)articleSelectedWithID:(NSString *)articleID;

@end


@class NavigationModel;
class NCXNavPoint;

@interface ArticleSelectorViewController : UITableViewController<UITableViewDelegate, UITableViewDataSource> {
	id<ArticleSelectorDelegate> delegate;
	
	NavigationModel *mNavModel;
	
	NCXNavPoint *mParentNavPoint;
}

@property(nonatomic, assign) id<ArticleSelectorDelegate> delegate;

- (void)setParentNavPoint:(NCXNavPoint *)parentNavPoint;
- (NCXNavPoint *)parentNavPoint;

@end
