#import "PDFView.h"

#import "StudentController.h"

@implementation PDFView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		self.backgroundColor = [UIColor whiteColor];
		NSArray *viewsToRemove = [self subviews];
		for (UIView *v in viewsToRemove) {
			[v removeFromSuperview];
		}
		pageNumber = 0;
    }
    return self;
}


-(void) nextPage
{
	pageNumber++;
}

-(void) previousPage
{
	pageNumber--;
}


- (void) setPdf:(NSString*) iPath
{
	//NSString* path  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iPath];
	myPageRef = CGPDFDocumentCreateWithURL((CFURLRef)[NSURL fileURLWithPath:iPath]);
	pageNumber = 1;
}

- (void)drawRect:(CGRect)rect 
{
	[self drawPDFinContext:UIGraphicsGetCurrentContext()];
}

-(void)drawPDFinContext:(CGContextRef)context
{
	// draw the pdf
	CGContextTranslateCTM(context, 0, 1024);
	CGContextScaleCTM(context, 1.0, -1.0);
	CGPDFPageRef page = CGPDFDocumentGetPage(myPageRef, pageNumber);
	CGContextSaveGState(context);
	CGAffineTransform pdfTransform = CGPDFPageGetDrawingTransform(page, kCGPDFCropBox, self.bounds, 0, true);
	CGContextConcatCTM(context, pdfTransform);
	CGContextDrawPDFPage(context, page);
	
	// draw a black rectangle around the pdf:
	/*
	CGContextSetLineWidth(context, 100); 
	CGContextScaleCTM(context, 1,2);
	CGContextBeginPath(context);
	CGContextAddRect(context, self.frame);
	CGContextStrokePath(context);
	*/
	CGContextRestoreGState(context);

}


- (void)dealloc
{
	[super dealloc];
}


@end
