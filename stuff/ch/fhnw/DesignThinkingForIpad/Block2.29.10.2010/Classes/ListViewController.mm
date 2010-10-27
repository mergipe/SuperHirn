//
//  ListViewController.mm
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import "ListViewController.h"
#import "StudentController.h"


@implementation ListViewController


@synthesize _table;
@synthesize _frontImage;
@synthesize delegate;

- (void)viewDidLoad 
{
	[super viewDidLoad];
	listOfFiles = [StudentController getFiles];
	
	// define which image is used for the front image
	_frontImage.image = [UIImage imageNamed:@"icon.jpg"];
		
	//_table.separatorStyle = UITableViewCellSeparatorStyleNone;
    _table.rowHeight = 50;
    _table.backgroundColor = [UIColor blueColor];

    // create a new subview for the header of the table:
    UIView *containerView = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 60)] autorelease];
	containerView.backgroundColor = [UIColor greenColor];
   
	// create a lable for the table header:
	UILabel *headerLabel = [[[UILabel alloc] initWithFrame:CGRectMake(10, 20, 768, 40)] autorelease];
    headerLabel.text = @"My iCompetence Files";
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
	int index = [indexPath indexAtPosition:1];
	NSLog(@"ListViewController: tapped on list item at %d", index);
	
	// notify the registered delegate:
	[delegate didSelectItem:index];
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	return [listOfFiles count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	// create a table cell:
	UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
									  reuseIdentifier:@""];
	// Set the text of the cell to the row index.
	cell.textLabel.text = [listOfFiles objectAtIndex:indexPath.row];
	cell.textLabel.font = [UIFont boldSystemFontOfSize:50];
	return cell;
}

- (void)dealloc 
{
	[_fontImage release];
	[listOfFiles release];
	[super dealloc];
}


@end
