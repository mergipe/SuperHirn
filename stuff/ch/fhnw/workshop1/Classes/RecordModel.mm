//
//  RecordModel.mm
//
//  Authors: SH/LM on 07/06/10.
//  
//

#import "RecordModel.h"

#import "MainPadAppDelegate.h"

static RecordModel *sharedInstance = nil;

@implementation RecordModel

@synthesize RecordList;
@synthesize titleList;
@synthesize issueList;

+ (RecordModel *)sharedModel {
    @synchronized(self) {
        if (sharedInstance == nil) {
			sharedInstance = [[RecordModel alloc] init];
			sharedInstance.RecordList = [[NSMutableArray alloc] init];
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

- (void)loadOverviewRecords {
	//Clear the existing Record
	[RecordList removeAllObjects];
	[titleList removeAllObjects];
	[issueList removeAllObjects];
	
	//Get the Record path first
	MainPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *RecordPath = [appDelegate RecordBasePath];
	
	NSFileManager *fManager = [NSFileManager defaultManager];
	NSArray *mags = [fManager RecordsOfDirectoryAtPath:RecordPath error:nil];
	NSString *magazine;
	for(magazine in mags) {
		NSString *magPath = [RecordPath stringByAppendingPathComponent:magazine];
		NSArray *magFiles = [fManager RecordsOfDirectoryAtPath:magPath error:nil];
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
				[RecordList addObject:coverImgPath];
				
					//NSLog(@"Adding magazin path= %@ with title %@", coverImgPath, title);
				
				break;
			}
		}
	}
}

@end
