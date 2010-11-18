

#import <UIKit/UIKit.h>


#import <UIKit/UIKit.h>
#import "ModuleBottomNavController.h"

@interface ModuleController : UIViewController<UIScrollViewDelegate, BottomNavigationDelegate> 
{
	IBOutlet UIScrollView *mScrollView;	
	NSMutableArray* allSwipeItems;
	int currentItemInView;
	ModuleBottomNavController *bottomNavViewController;
	NSString* title;
}

/**
 * Set the title of this swipe view
 * @param iTitle NSString*
 */
- (void)setTitle:(NSString*) iTitle;

/**
 * Get the title of this swipe view
 * @return NSString*
 */
- (NSString*)title;


/**
 * Hide the bottom navigation view
 */
- (void)hideBottomNavigation;

/**
 * Show the bottom navigation view
 */
- (void)showBottomNavigation;

/**
 * Centers the swipe view at this index
 * @param iItemIndex int
 * @author Lukas Mueller
 */
-(void) centerAtItem:(int)iItemIndex;

/**
 * Initialization function to load the images into the swipe view
 * @author Lukas Mueller
 */
-(void)setUpSwipeView;

/**
 * Set up of the bottom navigation view
 * @author Lukas Mueller
 */
- (void)setupBottomNavigationView;

/**
 * Add a new view to the swipe view
 * @param iView UIView*
 * @author Lukas Mueller
 */
-(void)addSwipeItem:(UIView*)iView:(NSString*) name: (NSString*) iconPath;


@end
