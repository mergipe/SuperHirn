//
//  ListViewController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>
#import "ContactDetail.h"

@class MFMailComposeViewController;
@interface ListViewController : UIViewController <MFMailComposeViewControllerDelegate,UITableViewDataSource, UITableViewDelegate, SendEmailDelegate> 
{
	
@public
	UITableView* _table;
	MFMailComposeViewController* mailComposeController;

	
		
}

@property (nonatomic, retain) IBOutlet UITableView* _table;


@end
