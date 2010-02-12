/*
 *  R : A Computer Langage for Statistical Data Analysis
 *  Copyright (C) 1996  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1999-2002   Robert Gentleman, Ross Ihaka and the
 *                            R Development Core Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

////////////////////////////////////////////////////////////////////////
///// functions added by Lukas Mueller:

double fmax2(double, double);

int imax2(int, int);

int imin2(int, int);


///////////////////////////////////////////////////////////////
//   Partial Sorting of double!:
//
// Partial sort so that x[k] is in the correct place, smaller to left,
// larger to right
//
// NOTA BENE:  k < n  required, and *not* checked here but in do_psort();
// -----  infinite loop possible otherwise!
// x* : array of double
// n  : length of array
// k  : element up to where sorted

void rPsort(double*, int , int);
////
////////////////////////////////////////////////////////////////////////


double fsquare(double);

double fcube(double);


//////////////////////////////////////////////////////
// help function lowest:

//void lowest(double *x, double *y, int n, double *xs, double *ys,int nleft, int nright, double *w, Rboolean userw, double *rw, Rboolean *ok);
//void clowess(double *x, double *y, int n,double f, int nsteps, double delta, double *ys, double *rw, double *res);
    

///////////////////////////////////////////////////////////////
// lowess main function to call

void lowess(double *x, double *y, int *n, double *f, int *nsteps, double *delta, double *ys, double *rw, double *res);
