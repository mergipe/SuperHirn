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

- (void)viewDidLoad 
{
	[super viewDidLoad];
	listOfFiles = [StudentController getFiles];
}




- (void)dealloc 
{
	[listOfFiles release];
	[super dealloc];
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	int index = [indexPath indexAtPosition:1];
	NSLog(@"Tapped on list item at %d", index);
	NSDictionary *dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:index] forKey:@"itemIndex"];
	[[NSNotificationCenter defaultCenter] postNotificationName:@"showFullScreenItemView" object:self userInfo:dict];
	
	
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	return [listOfFiles count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	// Identifier for retrieving reusable cells.
	static NSString *cellIdentifier = @"File";
	
	// Attempt to request the reusable cell.
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
	
	// No cell available - create one.
	if(cell == nil) {
		cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
									  reuseIdentifier:cellIdentifier];
	}
	
	// Set the text of the cell to the row index.
	cell.textLabel.text = [listOfFiles objectAtIndex:indexPath.row];
	return cell;
}

@end
