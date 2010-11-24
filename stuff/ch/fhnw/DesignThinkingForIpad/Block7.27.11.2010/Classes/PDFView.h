
#import <UIKit/UIKit.h>

#import "Item.h"


@interface PDFView : Item 
{
	
@private
	CGPDFDocumentRef myPageRef;
	int pageNumber;
}

/**
 * Set the path to the pdf.
 * @param iPath NSString*
 * @author Lukas Mueller
 */
- (void) setPdf:(NSString*) iPath;

-(void) nextPage;
-(void) previousPage;



-(void)drawPDFinContext:(CGContextRef)context;


@end
