//
//  RecordSelectorViewController.mm
//  iCompetence
//
//  Authors: SH/LM on 18/05/10.
//  
//

#import "RecordSelectorViewController.h"



@implementation RecordSelectorViewController

@synthesize delegate;

 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
    }
    return self;
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	

	self.navigationItem.title = @"Record";
	
	self.clearsSelectionOnViewWillAppear = NO;
	self.contentSizeForViewInPopover = CGSizeMake(320.0, 460.0);
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
    return YES;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


/*
- (UIImage *)thumbnailImageForNavPoint:(NCXNavPoint *)navPoint {
	const EPubReader *reader = [mNavModel epubReader];
	
	QMap<QString, OPFItem *> ocfMap = reader->manifestMap();
	OPFItem *item = ocfMap.value(navPoint->thumbnailID());
	if(item == NULL)
		return nil;
	
	NSString *relativePath = [NSString stringWithUTF8String:item->href().toUtf8().data()];
	iCompetenceAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	
	NSString *imgPath = [basePath stringByAppendingPathComponent:relativePath];
	NSLog(@"Thumbanail Image Path = %@", imgPath);
	
	return [UIImage imageWithRecordsOfFile:imgPath];
}
*/

#pragma mark UITableView delegate/data source METHODS
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	return 120.0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	/*
	if(mParentNavPoint == NULL) {
		return [mNavModel topLevelItemCount];
	}
	*/
	return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *identity = @"RecordCell";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identity];
	
	UIImageView *thumbnailView;
	UILabel *titleLabel;
	
	if(cell == nil) {
		cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identity];
		
		thumbnailView = [[UIImageView alloc] initWithFrame:CGRectMake(10.0, 10.0, 75.0, 100.0)];
		thumbnailView.tag = 1;
		[cell addSubview:thumbnailView];
		[thumbnailView release];
		
		titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(100.0, 10.0, 200.0, 15.0)];
		titleLabel.tag = 2;
		[cell addSubview:titleLabel];
		[titleLabel release];
		
	}
	else {
		thumbnailView = (UIImageView *)[cell viewWithTag:1];
		titleLabel = (UILabel *)[cell viewWithTag:2];
	}
	
	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
	
}

@end
