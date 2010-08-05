//
//  RecordNavigationViewController.h
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import <UIKit/UIKit.h>

@class RecordWebView;

@interface RecordNavigationViewController : UIViewController<UIScrollViewDelegate, UIWebViewDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	NSMutableArray *RecordPathList;
	
	RecordWebView *currentCover;
	RecordWebView *nextCover;
	
	NSString *RecordCoverPath;
}

@property(nonatomic, retain) NSString *RecordCoverPath;

- (id)initWithMagazinePath:(NSString *)magPath;

@end
