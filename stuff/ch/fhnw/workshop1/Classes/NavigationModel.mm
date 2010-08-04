//
//  NavigationModel.mm
//  AkiliPad
//
//  Created by Mithin on 18/05/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "NavigationModel.h"
#import "AkiliPadAppDelegate.h"

#include <QString>
#include <QList>
#include <QMap>
#include <QListIterator>
#include <QDebug>

#include "EPubReader.h"
#include "OPFItem.h"
#include "NCXNavMap.h"
#include "NCXNavPoint.h"

static NavigationModel *sharedInstance = nil;

@implementation NavigationModel

@synthesize currentSpineIndex;

+ (NavigationModel *)sharedModel {
    @synchronized(self) {
        if (sharedInstance == nil) {
			sharedInstance = [[NavigationModel alloc] init];
			sharedInstance.currentSpineIndex = 0;
			[sharedInstance resetEPUBReader];
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

- (void)resetEPUBReader {
	mEPUBReader = NULL;
}

- (void)createEPUBReader {
	if(mEPUBReader == NULL) {
		mEPUBReader = new EPubReader;
	}
	else {
		delete mEPUBReader;
		mEPUBReader = NULL;
		[self createEPUBReader];
	}
}

- (EPubReader *&)epubReader {
	return mEPUBReader;
}

- (const NCXNavMap &)navMap {
	return mEPUBReader->navMap();
}

/*- (const QString &)navMapCoverID {
	const NCXNavMap &navMap = mEPUBReader->navMap();
	return navMap.coverID();
}*/

- (void)printNavPoint:(NCXNavPoint *)navPoint {
	/*qDebug() << "Title = " << navPoint->title() << "\n";
	qDebug() << "Sub Title = " << navPoint->subTitle() << "\n";
	qDebug() << "Nav Point = " << navPoint->navPointID() << "\n";
	qDebug() << "Icon ID = " << navPoint->iconID() << "\n";
	qDebug() << "Thumbnail = " << navPoint->thumbnailID() << "\n";
	qDebug() << "Cover  = " << navPoint->coverID() << "\n";
	
	QList<NCXNavPoint *> childNavPoints = navPoint->childrenNavPoints();
	QListIterator<NCXNavPoint *> i(childNavPoints);
	while(i.hasNext()) {
		NCXNavPoint *navPoint = i.next();
		[self printNavPoint:navPoint];
	}*/
}

- (void)saveArticleCoverNavPointFrom:(NCXNavPoint *)navPoint {
	QList<NCXNavPoint *> childNavPoints = navPoint->childrenNavPoints();
	
	if(childNavPoints.count() !=	0) {
		if(navPoint->title().length() || navPoint->subTitle().length() || 
		   navPoint->iconID().length() || navPoint->thumbnailID().length()) {
			//Find the correct index where this belongs
			const QList<QString> spines = mEPUBReader->spineList();
			for(int i = 1; i < spines.count(); i++) {
				if(navPoint->navPointID() == spines.at(i)) {
					if(i-1 < topLevelNavPoints.count()) {
						topLevelNavPoints.insert(i-1, navPoint);
					}
					else {
						topLevelNavPoints.append(navPoint);
					}
					break;
				}
			}
		}
	}
	
	QListIterator<NCXNavPoint *> i(childNavPoints);
	while(i.hasNext()) {
		NCXNavPoint *cNavPoint = i.next();
		[self saveArticleCoverNavPointFrom:cNavPoint];
	}
}

- (void)loadNavigationFor:(NSString *)fPath {
	//Reset the spine index
	self.currentSpineIndex = 0;
	topLevelNavPoints.clear();
	
	QString path([fPath UTF8String]);
	[self createEPUBReader];
	mEPUBReader->load(path);
	
	const NCXNavMap &navMap = mEPUBReader->navMap();
	QList<NCXNavPoint *> navPoints = navMap.navPoints();
	QListIterator<NCXNavPoint *> i(navPoints);
	while(i.hasNext()) {
		NCXNavPoint *navPoint = i.next();
		[self printNavPoint:navPoint];
		[self saveArticleCoverNavPointFrom:navPoint];
	}
	/*QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	QList<QString>keys = ocfMap.keys();
	for(int i = 0; i < keys.count(); i++) {
		QString key = keys[i];
		//qDebug() << "Key = " << key;
		OPFItem *item = ocfMap.value(key);
		//qDebug() << "HREF = " << item->href();
	}
	const QList<QString> spines = mEPUBReader->spineList();
	for(int i = 0; i < spines.count(); i++) {
		//qDebug() << "SPINE = " << spines.at(i);
	}*/
}

- (int)topLevelItemCount {
	const NCXNavMap &navMap = mEPUBReader->navMap();
	QList<NCXNavPoint *> navPoints = navMap.navPoints();
	
	return navPoints.count();
}

- (NCXNavPoint *)navPointAtIndex:(int)index {
	const NCXNavMap &navMap = mEPUBReader->navMap();
	QList<NCXNavPoint *> navPoints = navMap.navPoints();
	
	if(index < 0 || index >= navPoints.count()) {
		return NULL;
	}

	return navPoints[index];
}

- (NSString *)opfItemIDForFilePath:(NSString *)filePath {
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	NSString *opfItemID = nil;
	//First, find the right key from the values of the map
	QList<OPFItem *> items = ocfMap.values();
	for(int i = 0; i < items.count(); i++) {
		OPFItem *item = items[i];
		NSString *href = [NSString stringWithUTF8String:item->href().toUtf8().data()];
		if([filePath hasSuffix:href]) {
			opfItemID = [NSString stringWithUTF8String:ocfMap.key(item).toUtf8().data()];
			break;
		}
	}
	return opfItemID;
}

- (NSString *)coverFileName {
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	const NCXNavMap &navMap = mEPUBReader->navMap();
	OPFItem *item = ocfMap.value(navMap.coverID());
	
	if(item == NULL)
		return nil;
	
	return [NSString stringWithUTF8String:item->href().toUtf8().data()];
}

- (NSString *)fallbackFileName {
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	const NCXNavMap &navMap = mEPUBReader->navMap();
	OPFItem *item = ocfMap.value(navMap.fallback());
	
	if(item == NULL)
		return nil;
	
	return [NSString stringWithUTF8String:item->href().toUtf8().data()];
}

- (void)setSpineIndexForItemId:(NSString *)itemID {
	QString filePathID([itemID UTF8String]);
	//We have the id. Now find out the index
	const QList<QString> spines = mEPUBReader->spineList();
	for(int i = 0; i < spines.count(); i++) {
		if(spines[i] == filePathID) {
			//NSLog(@"Setting current spine index to %d", i);
			currentSpineIndex = i;
			break;
		}
	}
}

- (void)setSpineIndexForArticleFilePath:(NSString *)filePath {
	NSString *opfItemID = [self opfItemIDForFilePath:filePath];
	if(opfItemID == nil) {
		//Something has gone wrong, we are not able to find out the
		//spine index
		return;
	}
	[self setSpineIndexForItemId:opfItemID];
}

- (NSString *)filePathForNavPointWithId:(NSString *)navPointId {
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	
	QString key([navPointId UTF8String]);
	OPFItem *item = ocfMap.value(key);
	NSString *href = [NSString stringWithUTF8String:item->href().toUtf8().data()];
	
	return [basePath stringByAppendingPathComponent:href];
}

- (int)pageCountForArticleWithPath:(NSString *)articlePath {
	const QList<QString> spines = mEPUBReader->spineList();
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	
	//Start from current spine index+1 find out how many entries are in the same folder
	int count = 0;
	NSString *articleBasePath = [articlePath stringByDeletingLastPathComponent];
	for(int i = currentSpineIndex; i < spines.count(); i++) {
		OPFItem *item = ocfMap.value(spines[i]);
		NSString *href = [NSString stringWithUTF8String:item->href().toUtf8().data()];
		NSString *bPath = [basePath stringByAppendingPathComponent:[href stringByDeletingLastPathComponent]];
		if([articleBasePath isEqualToString:bPath]) {
			count++;
		}
		else {
			break;
		}
	}
	//NSLog(@"Article page count = %d", count);
	return count;
}

- (QList<NCXNavPoint *> &)topLevelNavPointList {
	return topLevelNavPoints;
}

- (NSString *)filePathForItemWithId:(NSString *)itemID {
	AkiliPadAppDelegate *appDelegate = [UIApplication sharedApplication].delegate;
	NSString *basePath = appDelegate.latestIssuePath;
	QMap<QString, OPFItem *> ocfMap = mEPUBReader->manifestMap();
	QString key([itemID UTF8String]);
	OPFItem *item = ocfMap.value(key);
	if(item == NULL)
		return nil;
	
	NSString *href = [NSString stringWithUTF8String:item->href().toUtf8().data()];
	return [basePath stringByAppendingPathComponent:href];
}

@end
