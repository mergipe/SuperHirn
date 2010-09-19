//
//  RootViewController.m
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import "RootViewController.h"
#import "DetailViewController.h"
#import "StudentController.h"

@implementation RootViewController

@synthesize detailViewController;


#pragma mark -
#pragma mark View lifecycle

NSMutableArray *listOfPictures;

- (void)viewDidLoad {   
	listOfPictures = [StudentController getPictures];
 	
    //---set the title---
    self.navigationItem.title = @"Pictures";    
	
    [super viewDidLoad];
    self.clearsSelectionOnViewWillAppear = NO;
    self.contentSizeForViewInPopover = CGSizeMake(320.0, 600.0);
	
}

- (NSInteger)tableView:(UITableView *)aTableView 
 numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    //return 10;
    return [listOfPictures count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView 
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
    static NSString *CellIdentifier = @"CellIdentifier";
	
    // Dequeue or create a cell of the appropriate type.
    UITableViewCell *cell = [tableView 
							 dequeueReusableCellWithIdentifier:CellIdentifier];
	
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] 
				 initWithStyle:UITableViewCellStyleDefault 
				 reuseIdentifier:CellIdentifier] autorelease];
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
	
    // Configure the cell.    
    // cell.textLabel.text = 
    //     [NSString stringWithFormat:@"Row %d", indexPath.row];
	
    cell.textLabel.text = [listOfPictures objectAtIndex:indexPath.row];
	
    return cell;
}

- (void)tableView:(UITableView *)aTableView 
didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
    /*
     When a row is selected, set the detail view controller's detail item to 
     the item associated with the selected row.
     */
    //detailViewController.detailItem = 
    //    [NSString stringWithFormat:@"Row %d", indexPath.row];
	
    detailViewController.detailItem = 
	[NSString stringWithFormat:@"%@", 
	 [listOfPictures objectAtIndex:indexPath.row]];    
}

- (void)dealloc {
    [listOfPictures release];
    [super dealloc];
}
@end

