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



@interface ListViewController : UIViewController <MFMailComposeViewControllerDelegate,UITableViewDataSource, UITableViewDelegate> 
{
	
@public
	UITableView* _table;
	
		
}

@property (nonatomic, retain) IBOutlet UITableView* _table;


@end
