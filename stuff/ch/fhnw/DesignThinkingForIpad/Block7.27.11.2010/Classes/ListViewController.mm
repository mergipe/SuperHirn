//
//  ListViewController.mm
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import "ListViewController.h"

#import "ContactDetail.h"


@implementation ListViewController


@synthesize _table;

- (void)viewDidLoad 
{
	[super viewDidLoad];
	
	//_table.separatorStyle = UITableViewCellSeparatorStyleNone;
    _table.rowHeight = 50;
    _table.backgroundColor = [UIColor whiteColor];
	
    // create a new subview for the header of the table:
    UIView *containerView = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 60)] autorelease];
	containerView.backgroundColor = [UIColor grayColor];
	
	// create a lable for the table header:
	UILabel *headerLabel = [[[UILabel alloc] initWithFrame:CGRectMake(10, 20, 768, 40)] autorelease];
    headerLabel.text = @"My Contacts";
    headerLabel.textColor = [UIColor whiteColor];
    headerLabel.shadowColor = [UIColor blackColor];
	headerLabel.textAlignment =  UITextAlignmentCenter;
    headerLabel.font = [UIFont boldSystemFontOfSize:22];
    headerLabel.backgroundColor = [UIColor clearColor];
	
    // add the label to the header view
	[containerView addSubview:headerLabel];
	
	// set the header view in the tabl
    _table.tableHeaderView = containerView;
	
	
	
}




- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	
	ContactDetail* myItem = [[[ContactDetail alloc] initWithFrame:CGRectMake(0, 0, 770, 960)] autorelease];
	myItem.emailDelegate = self;
	[self.view addSubview:myItem];
	
	
	// create a new subview for the popover of the table:
	//  UIView *containerView2 = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 770, 960)] autorelease];
	//containerView2.backgroundColor = [UIColor colorWithWhite:0.000 alpha:0.600];
	//[self.view addSubview:containerView2];
	
	// create a new subview for the popover2 of the table:
	//  UIView *containerView3 = [[[UIView alloc] initWithFrame:CGRectMake(80, 100, 600, 800)] autorelease];
	//	containerView3.backgroundColor = [UIColor colorWithWhite:1.000 alpha:0.600];
	//	[self.view addSubview:containerView3];
	
	//_table.tableHeaderView = containerView2;
	
		
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	return 10;
}


- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	// create a table cell:
	UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
												   reuseIdentifier:@""];
	// Set the text of the cell to the row index.
	cell.textLabel.text = @"Max Haueter";
	cell.textLabel.font = [UIFont boldSystemFontOfSize:50];
	return cell;
}


#pragma mark MFMailComposeViewController delegate methods
- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result 
						error:(NSError*)error {
	
	[self dismissModalViewControllerAnimated:YES];
}


-(void)sendEmail
{
	
	
	mailComposeController = [[MFMailComposeViewController alloc] init];
	mailComposeController.mailComposeDelegate = self;
	
	NSString* header = @"Sending Email from iCompetence App";
	NSString *content = @"<br/><br/>iCompetence App empowered by year 2010";

	[mailComposeController setSubject: header];
	[mailComposeController setMessageBody:content isHTML:YES];
	
	[self presentModalViewController:mailComposeController animated:YES];}

- (void)dealloc 
{
	[_table dealloc];
	[super dealloc];
}


@end
