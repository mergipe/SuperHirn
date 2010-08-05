//
//  RecordModel.h
//
//  Created by LM/SH
//

#import <Foundation/Foundation.h>

#define MAGAZINE_COUNT_PER_PAGE	9

@interface RecordModel : NSObject {
	NSMutableArray *RecordList;
	NSMutableArray *issueList;
	NSMutableArray *titleList;
}

+ (RecordModel *)sharedModel;

@property(nonatomic, retain) NSMutableArray *RecordList;
@property(nonatomic, retain) NSMutableArray *titleList;
@property(nonatomic, retain) NSMutableArray *issueList;
		  
- (void)loadOverviewRecords;

@end
