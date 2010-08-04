//
//  LibraryModel.mm
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "LibraryModel.h"

#import "MainPadAppDelegate.h"

static LibraryModel *sharedInstance = nil;

@implementation LibraryModel

@synthesize contentList;
@synthesize titleList;
@synthesize issueList;

+ (LibraryModel *)sharedModel {
    @synchronized(self) {
        if (sharedInstance == nil) {
			sharedInstance = [[LibraryModel alloc] init];
			sharedInstance.contentList = [[NSMutableArray alloc] init];
			sharedInstance.titleList = [[NSMutableArray alloc] init];
			sharedInstance.issueList = [[NSMutableArray alloc] init];
		}
    }
    return sharedInstance;
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedInstance == nil) {
            sharedInstance = [super allocWithZone:zone];
            return sharedInstance;  // assignment and return on first allocation
        }
    }
    return nil; // on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone {
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
    //do nothing
}

- (id)autorelease {
    return self;
}

- (void)loadLibraryContents {
	//Clear the existing content
	[contentList removeAllObjects];
	[titleList removeAllObjects];
	[issueList removeAllObjects];
	
	//Get the content path first
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *contentPath = [appDelegate contentBasePath];
	
	NSFileManager *fManager = [NSFileManager defaultManager];
	NSArray *mags = [fManager contentsOfDirectoryAtPath:contentPath error:nil];
	NSString *magazine;
	for(magazine in mags) {
		NSString *magPath = [contentPath stringByAppendingPathComponent:magazine];
		NSArray *magFiles = [fManager contentsOfDirectoryAtPath:magPath error:nil];
		NSString *file;
		for(file in magFiles) {
			if([file hasPrefix:@"icon"] && ![file hasSuffix:@"html"]) {
				NSString *coverImgPath = [magPath stringByAppendingPathComponent:file];
				
				
					////////
					// Lukas: only temporary static string for issue and title desciption. Should be extracted afterwards from the epub
				NSRange foundRange = [magazine rangeOfString:@"old"];
				if( foundRange.length > 0	 )
				{
					[issueList addObject:  @"2010/20"];
					[titleList addObject:  @"Schule im Reformwahn"];
				}
				else
				{
					[issueList addObject:  @"2010/21"];
					[titleList addObject:  @"Der schwarze Turm"];
				}
				
					////////
				[contentList addObject:coverImgPath];
				
					//NSLog(@"Adding magazin path= %@ with title %@", coverImgPath, title);
				
				break;
			}
		}
	}
}

@end
