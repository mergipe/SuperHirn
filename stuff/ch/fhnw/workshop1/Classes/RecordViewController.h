//
//  MagazineViewController.h
//  iCompetence
//
//  Authors: SH/LM on 21/04/10.
//  
//

#import <UIKit/UIKit.h>
#import "RecordSelectorViewController.h"

#import "Window.h"

@class UIPopoverController;
@class RecordWebView;

@interface RecordViewController : UIViewController<UIWebViewDelegate, RecordSelectorDelegate, 
															UIScrollViewDelegate, WindowDelegate> {
	IBOutlet UIScrollView *mScrollView;
	
	Window *mWindow;
																
	RecordSelectorViewController *RecordSelector;
	UIPopoverController *RecordSelectorPopover;
	
	NSMutableArray *verticalRecordPathList;
	NSMutableArray *horizontalRecordPathList;
																
	RecordWebView *currentPage;
	RecordWebView *nextPage;

																
	int pageCount;
	int currentRecordIndex;
	BOOL viewPushed;
}


/**
 Checks if a image gallery loading is required based on the device orientation
 @author Lukas Mueller
 @returns BOOL true if gallery is requried, else false.
 */
- (BOOL)interfaceOrientationLandscape;


@property(nonatomic, retain) RecordSelectorViewController *RecordSelector;
@property(nonatomic, retain) UIPopoverController *RecordSelectorPopover;

@property(nonatomic) int pageCount;

- (IBAction)goBack:(id)sender;
- (IBAction)showTOC:(id)sender;

@end
