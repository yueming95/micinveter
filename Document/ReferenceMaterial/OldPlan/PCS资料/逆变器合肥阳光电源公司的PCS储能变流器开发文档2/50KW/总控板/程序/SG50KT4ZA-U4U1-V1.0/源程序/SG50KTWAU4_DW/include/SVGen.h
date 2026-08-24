/* =================================================================================
File name:       SVGEN.H  (IQ version)                    
                    
Originator:	

Description: 
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2008-08-22	Version 0.1                                                 
------------------------------------------------------------------------------*/
#ifndef _SVGEN_H
#define _SVGEN_H

typedef struct 	{ _iq  Ualpha; 	 //Input: reference alpha-axis phase voltage 
				  _iq  Ubeta;	 //Input: reference beta-axis phase voltage 
				  _iq  Ta;		 //Output:reference phase-a switching function		
				  _iq  Tb;		 //Output:reference phase-b switching function 
				  _iq  Tc;		 //Output:reference phase-c switching function
				  void (*calc)();//Pointer to calculation function
				} SVGEN;
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																				
typedef SVGEN *SVGEN_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SVGEN object.
-----------------------------------------------------------------------------*/                     
#define SVGEN_DEFAULTS { 0,0,0,0,0, \
                       (void (*)(unsigned long))svgen_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in SVGEN.C
------------------------------------------------------------------------------*/
void svgen_calc(SVGEN_handle);

#endif // _SVGEN_H_
