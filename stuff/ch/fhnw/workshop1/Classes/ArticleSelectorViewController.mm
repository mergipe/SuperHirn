//
//  ArticleSelectorViewController.mm
//  AkiliPad
//
//  Created by Mithin on 18/05/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "ArticleSelectorViewController.h"
#import "AkiliPadAppDelegate.h"

#import "NavigationModel.h"

#import "SubTitleLabel.h"

#include <QDebug>
#include <QMap>

#include "NCXNavPoint.h"
#include "EPubReader.h"
#include "OPFItem.h"

@implementation ArticleSelectorViewController

@synthesize delegate;

 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		mNavModel = [NavigationModel sharedModel];
		mParentNavPoint = NULL;
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
	
	if(mParentNavPoint == NULL) {
		self.navigationItem.title = @"Content";
	}
	else {
		self.navigationItem.title = [NSString stringWithUTF8String:mParentNavPoint->title().toUtf8().data()];
	}
	mNavModel = [NavigationModel sharedModel];
	
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

- (void)setParentNavPoint:(NCXNavPoint *)parentNavPoint {
	mParentNavPoint = parentNavPoint;
}

- (NCXNavPoint *)parentNavPoint {
	return mParentNavPoint;
}

- (NCXNavPoint *)navPointForIndexPath:(NSIndexPath *)indexPath {
	if(mParentNavPoint == NULL) {
		return [mNavModel navPointAtIndex:indexPath.row];
	}
	
	//return mParentNavPoint->childrenNavPoints().at(indexPath.row);
	QList<NCXNavPoint *> navPoints = mParentNavPoint->childrenNavPoints();
	int count = 0;
	for(int i = 0; i < navPoints.count(); i++) {
		NCXNavPoint *navPoint = navPoints[i];
		if(navPoint->title().length() || navPoint->subTitle().length() || 
		   navPoint->iconID().length() || navPoint->thumbnailID().length()) {
			if(count == indexPath.row)
				return navPoint;
			
			count++;
		}
	}
	
	return 0;
}

- (UIImage *)thumbnailImageForNavPoint:(NCXNavPoint *)navPoint {
	const EPubReader *reader = [mNavModel epubReader];
	
	QMap<QString, OPFItem *> ocfMap = reader->manifestMap();
	OPFItem *item = ocfMap.value(navPoint->thumbnailID());
	if(item == NULL)
		return nil;
	
	NSString *relativePath = [NSString stringWithUTF8String:item->href().toUtf8().data()];
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	
	NSString *imgPath = [basePath stringByAppendingPathComponent:relativePath];
	NSLog(@"Thumbanail Image Path = %@", imgPath);
	
	return [UIImage imageWithContentsOfFile:imgPath];
}

- (int)articleCoverCount {
	QList<NCXNavPoint *> navPoints = mParentNavPoint->childrenNavPoints();
	int count = 0;
	for(int i = 0; i < navPoints.count(); i++) {
		NCXNavPoint *navPoint = navPoints[i];
		if(navPoint->title().length() || navPoint->subTitle().length() || 
		   navPoint->iconID().length() || navPoint->thumbnailID().length()) {
			count++;
		}
	}
	return count;
}

#pragma mark UITableView delegate/data source METHODS
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	return 120.0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if(mParentNavPoint == NULL) {
		return [mNavModel topLevelItemCount];
	}
	
	return [self articleCoverCount];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *identity = @"ArticleCell";
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identity];
	
	UIImageView *thumbnailView;
	UILabel *titleLabel;
	SubTitleLabel *subTitleLabel;
	
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
		
		subTitleLabel = [[SubTitleLabel alloc] initWithFrame:CGRectMake(100.0, 35.0, 200.0, 80.0)];
		subTitleLabel.tag = 3;
		subTitleLabel.font = [UIFont systemFontOfSize:12];
		subTitleLabel.textColor = [UIColor grayColor];
		subTitleLabel.numberOfLines = 0;

		[cell addSubview:subTitleLabel];
		[subTitleLabel release];
	}
	else {
		thumbnailView = (UIImageView *)[cell viewWithTag:1];
		titleLabel = (UILabel *)[cell viewWithTag:2];
		subTitleLabel = (SubTitleLabel *)[cell viewWithTag:3];
	}
	
	NCXNavPoint *navPoint = [self navPointForIndexPath:indexPath];
	
	titleLabel.text = [NSString stringWithUTF8String:navPoint->title().toUtf8().data()];
	subTitleLabel.text = [NSString stringWithUTF8String:navPoint->subTitle().toUtf8().data()];
	thumbnailView.image = [self thumbnailImageForNavPoint:navPoint];
	
	if(navPoint->childrenNavPoints().count() > 0) {
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	}
	else {
		cell.accessoryType = UITableViewCellAccessoryNone;
	}
	
	return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	NCXNavPoint *navPoint = [self navPointForIndexPath:indexPath];
	if(navPoint->childrenNavPoints().count() > 0) {
		//Show children
		ArticleSelectorViewController *ac = [[ArticleSelectorViewController alloc] init];
		ac.delegate = self.delegate;
		[ac setParentNavPoint:navPoint];
		[self.navigationController pushViewController:ac animated:YES];
		[ac release];
	}
	else {
		//Open this article
		if(delegate != nil) {
			NCXNavPoint *navPoint = [self navPointForIndexPath:indexPath];
			NSString *navPointId = [NSString stringWithUTF8String:navPoint->navPointID().toUtf8().data()];
			[delegate articleSelectedWithID:navPointId];
		}
	}
}

@end
