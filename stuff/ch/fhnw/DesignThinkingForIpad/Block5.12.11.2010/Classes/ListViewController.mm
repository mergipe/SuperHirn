//
//  ListViewController.mm
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import "ListViewController.h"


@implementation ListViewController


@synthesize _table;

- (void)viewDidLoad 
{
	[super viewDidLoad];
		
	//_table.separatorStyle = UITableViewCellSeparatorStyleNone;
    _table.rowHeight = 50;
    _table.backgroundColor = [UIColor blueColor];

    // create a new subview for the header of the table:
    UIView *containerView = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 60)] autorelease];
	containerView.backgroundColor = [UIColor greenColor];
   
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
	UITableViewCell* cell =  [_table cellForRowAtIndexPath:indexPath];
	int index = [indexPath indexAtPosition:1];
	NSString* cellText = cell.textLabel.text;
	
	
	
	mailComposeController = [[MFMailComposeViewController alloc] init];
	mailComposeController.mailComposeDelegate = self;
	
	NSString* header = @"Sending Email to ";
	header = [header stringByAppendingString:cellText];
	[mailComposeController setSubject: header];
	[mailComposeController setMessageBody:@"Here is my Email signature" isHTML:YES];
	
	[self presentModalViewController:mailComposeController animated:YES];
	
	
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	
	return 2;
}


- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	// create a table cell:
	UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
									  reuseIdentifier:@""];
	// Set the text of the cell to the row index.
	cell.textLabel.text = @"Max Max";
	cell.textLabel.font = [UIFont boldSystemFontOfSize:50];
	return cell;
}


#pragma mark MFMailComposeViewController delegate methods
- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result 
						error:(NSError*)error {
	
	[self dismissModalViewControllerAnimated:YES];
	[mailComposeController release];
}

- (void)dealloc 
{

	[super _table];
	[super dealloc];
}


@end
