//
//  RecordSelectorViewController.h
//  iCompetence
//
//  Authors: SH/LM on 18/05/10.
//  
//

#import <UIKit/UIKit.h>

@protocol RecordSelectorDelegate

- (void)RecordSelectedWithID:(NSString *)RecordID;

@end


@interface RecordSelectorViewController : UITableViewController<UITableViewDelegate, UITableViewDataSource> {
	id<RecordSelectorDelegate> delegate;
	
}

@property(nonatomic, assign) id<RecordSelectorDelegate> delegate;


@end
