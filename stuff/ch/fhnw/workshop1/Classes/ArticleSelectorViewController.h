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


@interface ArticleSelectorViewController : UITableViewController<UITableViewDelegate, UITableViewDataSource> {
	id<ArticleSelectorDelegate> delegate;
	
}

@property(nonatomic, assign) id<ArticleSelectorDelegate> delegate;


@end
