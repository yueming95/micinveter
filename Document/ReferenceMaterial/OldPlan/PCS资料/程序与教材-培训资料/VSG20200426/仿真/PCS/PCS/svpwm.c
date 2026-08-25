/*  File    : csfunc.c
 *  Abstract:
 *
 *      Example C-file S-function for defining a continuous system.  
 *
 *      x' = Ax + Bu
 *      y  = Cx + Du
 *
 *      For more details about S-functions, see simulink/src/sfuntmpl_doc.c.
 * 
 *  Copyright 1990-2009 The MathWorks, Inc.
 *  $Revision: 1.1.6.1 $
 */


#define S_FUNCTION_NAME svpwm
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define U(element) (*uPtrs[element])  /* Pointer to Input Port0 */
//#define cPwmDeadTime 0.0347900390625
#define cPwmDeadTime 0
#define cMinPwm 0.0003411
#define uwInvCtrPWMPrd 4688
#define cMaxPwm 1
static real_T A[2][2]={ { -0.09, -0.01 } ,
                        {  1   ,  0    } 
                      };

static real_T B[2][2]={ {  1   , -7    } ,
                        {  0   , -2    } 
                      };

static real_T C[2][2]={ {  0   , 2     } ,
                        {  1   , -5    } 
                      };

static real_T D[2][2]={ { -3   , 0     } ,
                        {  1   , 0     } 
                      };

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 2);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 6);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    /* Take care when specifying exception free code - see sfuntmpl_doc.c */
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    Specifiy that we have a continuous sample time.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0,  125e-6);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);         
}

#define MDL_INITIALIZE_CONDITIONS
/* Function: mdlInitializeConditions ========================================
 * Abstract:
 *    Initialize both continuous states to zero.
 */
static void mdlInitializeConditions(SimStruct *S)
{
    real_T *x0 = ssGetRealDiscStates(S);
    int_T  lp;

    for (lp=0;lp<2;lp++) { 
        *x0++=0.0; 
    }
}



/* Function: mdlOutputs =======================================================
 * Abstract:
 *      y = Cx + Du 
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T            *y    = ssGetOutputPortRealSignal(S,0);
    real_T            *x    = ssGetRealDiscStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);
    const real_T *buf = ssGetInputPortRealSignal(S,0); 
    double Alaph,Belta,Ia,Ib,Ic,Busdiff;
  
    double wModuAf, wModuBt,wBusDiffCtrl;          //common referenc vector for all sector
    double wModuAfTemp,wModuBtTemp,wSqrt3ModuAfTemp,wSqrt3ModuBtTemp;
    double wtemp,wtemp1,wtemp2,wtemp3,wtemp4;//wtemp variables for select sector and region. every wtemp variable denotes a beeline
    double wIo1,wIo2;                 //neutral current is I01 for B,D,F region and I02 for ACE region           
    int wSector = 0,wRegion = 0;   //define sector variables,where there are 6 Regions.
    double wTa,wTb,wTc;               //vector dwelling time in the first sector
    double wTaQ1,wTaQ2,wTbQ1,wTbQ2,wTcQ1,wTcQ2;   //A phase, B phase, C phase duty in the first sector on Ts period
    double wFactor1,wFactor2;         //Q15  Bus voltage balance factor,wFactor1 + wFactor2 = 1
    double wPwmTaQ1,wPwmTaQ2,wPwmTbQ1,wPwmTbQ2,wPwmTcQ1,wPwmTcQ2;
    int  i=0;
    UNUSED_ARG(tid); /* not used in single tasking mode */

    /* y=Cx+Du */
    //y[0]=C[0][0]*x[0]+C[0][1]*x[1]+D[0][0]*U(0)+D[0][1]*U(1);
    //y[1]=C[1][0]*x[0]+C[1][1]*x[1]+D[1][0]*U(0)+D[1][1]*U(1);
    Alaph=U(0);
    Belta=U(1);
    Ia=U(2);
    Ib=U(3);
    Ic=U(4);
    Busdiff=U(5);
    
    
    wModuAfTemp=Alaph;
    wModuBtTemp=Belta;
    wSqrt3ModuAfTemp=sqrt(3)*wModuAfTemp;
    wSqrt3ModuBtTemp=sqrt(3)*wModuBtTemp;
    
    wtemp1 = wModuBtTemp;                //Ubeta 
    wtemp2 = wModuBtTemp - wSqrt3ModuAfTemp; //Ubeta - sqrt3*Ualta
    wtemp3 = wModuBtTemp + wSqrt3ModuAfTemp; //Ubeta + sqrt3*Ualta
    
    wFactor1 = 0.5+Busdiff;             //Bus Balance control value   
    wFactor2 = 0.5-Busdiff;
    
   
   
    /****Judge sector ******start*********/      
    if(wtemp1>=0)
    {
        if(wtemp2<=0)
        {
            wSector=1;
        }
        else 
        {
            if(wtemp3<=0) 
            { 
                wSector=3;
            }
            else
            {
                wSector=2;
            }
        }
    }
    if(wtemp1<0)
    {
        if(wtemp2>=0)
        {
            wSector=4;
        }
        else 
        {
        	  if(wtemp3>=0)
            {
            	  wSector=6;
            }
            else
            {
            	  wSector=5;
            }
        }
    }
    
    
     /*******<Step 1>***Judge sector ****End*************/
    
    /*******<Step 2>*******Referenc vectors in other sector transformed to first sector***********/
    wtemp1= wSqrt3ModuBtTemp/2;   // Ubeta*sqrt3/2    
    wtemp2= wSqrt3ModuAfTemp/2;   // Ualfa*sqrt3/2
    //ReadSector= wSector;//Tiger add 0910018
    
    switch (wSector)  
    {    
        case 1:
            wIo1 =  -Ia;  //for B,D,F Region
            wIo2 = Ic;    //for A,C,E Region 
            wModuAf = wModuAfTemp;                                         
            wModuBt = wModuBtTemp;	 		                                  
        break;    
                                                              
        case 2:   
            wIo1 =  -Ic;  // wModuAfTemp,wModuBtTemp
            wIo2 =  Ib;   //                                                               
            wModuAf = wModuAfTemp/2+wtemp1;     //   Ucos(theta-60) = Ualfa/2 + Ubeta*sqrt(3)/2
            wModuBt = wModuBtTemp/2-wtemp2;	    //   Usin(theta-60) = Ubeta/2 - Ualfa*sqrt(3)/2
        break;  
                                                                      
        case 3:   
            wIo1 = -Ib;   //
            wIo2 = Ia;    //                                                        //   
 		    wModuAf = -wModuAfTemp/2+wtemp1;     //   Ucos(theta-120) = -Ualfa/2 + Ubeta*sqrt(3)/2  
            wModuBt = -wModuBtTemp/2-wtemp2;     //   Usin(theta-120) = -Ubeta/2 - Ualfa*sqrt(3)/2  
        break; 
                                                                   
        case 4:   
            wIo1 =  -Ia; // 
            wIo2 = Ic;   //                                           
            wModuAf = -wModuAfTemp;                                                             //   Ucos(theta-180) = -Ualfa 
            wModuBt = -wModuBtTemp;                                                             //   Usin(theta-180) = -Ubeta 
        break;
                                                                                          
        case 5:  
            wIo1 = -Ic;  //
            wIo2 = Ib;   //                          
            wModuAf = -wModuAfTemp/2-wtemp1;     //
            wModuBt = -wModuBtTemp/2+wtemp2;     //
        break; 
                                        
        case 6:
            wIo1 = -Ib;
            wIo2 = Ia;
            wModuAf = wModuAfTemp/2-wtemp1;
            wModuBt = wModuBtTemp/2+wtemp2;  
        break;
        
        default:
            wIo1 = Ia;
            wIo2 = Ic;
            wModuAf = wModuAfTemp;
            wModuBt = wModuBtTemp;
        break;
    }
    
    
    /********<Step 2>********transformed *****End**********************/
    
    /********<Step 3>****Judge which region in the first sector *******/
    wtemp = wModuAf*sqrt(3);                          //    |     ** 3* *     |
    //wtemp2 = wtemp1;                                                       //    |    **--*--**    |
    wtemp1 = wtemp + wModuBt - 1;    //sqrt3*Ualfa + Ubeta - 0x3fff ;   //    |   **---2---**   |	              
    wtemp2 = wtemp - wModuBt - 1;    //sqrt3*Ualfa - Ubeta - 0x3fff ;   //    |  **----*----**  |
    wtemp3 = wModuBt - 0.5;            //Ubeta - 0x2000 ;                 //    | **1*********4** |
                                                          
    if(wtemp1<=0)  //(AB)
	{
	    wRegion = 1;
	}                                  
	else if(wtemp3>=0)//E
	{ 	                               
	    wRegion = 3;   
	}                                  
	
	else if(wtemp2>=0)  //   F
	{        
	    wRegion = 4; 
	}                                  
	else                               
	{    
	    wRegion = 2;    //CD
    }		
     /*y[0]=wModuAf;
    y[1]=wModuBt;
    y[2]=wtemp1;
    y[3]=wtemp2;
    y[4]=wtemp3;
    y[5]=wRegion;
    y[6] = wtemp;
    y[7] = wTaQ2;
    y[8] = wTbQ1;
    y[9] = wTbQ2;
    y[10]=wTcQ2;
    y[11]=wSector;
    
    /********<Step 3>*****Judge **********End *******/     
    
    /********<Step 4>****compute dwelling time based on the different regions ****Start*******/
    wtemp4 = wModuAf*sqrt(3)/3-wModuBt; 
    wtemp1 = wtemp - wModuBt; //sqrt3*Ualfa-Ubeta;
	wtemp2 = wtemp + wModuBt; //sqrt3*Ualfa+Ubeta;
    wtemp3 = wModuBt*2;      //2*Ubeta
    
    switch  (wRegion)
	{
	    case 1:
	        //wTa = wtemp1;
	        //wTb = 0x1fff-wtemp2;
	        //wTc = wtemp3;
	        //break;
	    case 2:
	    	if (wtemp4 > 0) //<30 degree C
	    	{
	            //wTa = 0x3fff-wtemp3;
	            //wTb = wtemp2-0x3fff;                      
                //wTc = 0x3fff-wtemp1;   
                
                wTa = 2-wtemp2;    //按第4小扇区处理          D   载波马鞍波头有一个小尖，为了去掉所以在这里处理
	            wTb = wtemp3;                      
                wTc = wtemp1-1;
                  
            }
            else  //>30 degree 
            {
                wTa = wtemp3-1;      //按第3小扇区处理            
		        wTb = wtemp1;                      
                wTc = 2-wtemp2;
            }		    			    	
		    
		    //wTa = 0x1fff-wtemp3;
		    //wTb = wtemp2-0x1fff;                      
            //wTc = 0x1fff-wtemp1;
            break;
        case 3:
            wTa = wtemp3-1;                
		    wTb = wtemp1;                      
            wTc = 2-wtemp2;   
        break;
        
        case 4:
            wTa = 2-wtemp2;                
		    wTb = wtemp3;                      
            wTc = wtemp1-1;
        break;
        
        default:
            wTa = wtemp1;
		    wTb = 2-wtemp2;
		    wTc = wtemp3;
	    break;     	                    
    }
    
   
    //Limiter of dwelling time           
    //if(wTa>0x1fff)
    //{
    //    wTa=0x1fff;
    //}
    //if(wTb>0x1fff)
    //{
    //    wTb=0x1fff;
    //}
    //if(wTc>0x1fff)
    //{
    //    wTc=0x1fff;
    //}       
    /********<Step 4>****compute dwelling time  **********End *******/ 
    
    /********<Step 5>****compute three phase switch active time (duty) for first sector ***Start*******/           
  wtemp1 = wModuAf*sqrt(3)/3-wModuBt;
   
    switch (wRegion)
    {              
        case 1:
            //if(wtemp1>0) // 30 degree bisector, >0 means lower space, theta <30 degress; <0 means higher space, theta >30 degress
            //{
            //    if(wIo1<0)  // swap bus voltage balance factor
            //    {
            //        wtemp = wFactor1;
            //        wFactor1 = wFactor2;
            //        wFactor2 = wtemp; 
            //    }
            //    wTaQ1 = (INT16S)(((INT32S)wTa*wFactor1)>>15);
            //    wTaQ2 = 0x1fff;
            //    wTbQ1 = 0;
            //    wTbQ2 = 0x1fff-(INT16S)(((INT32S)wTa*wFactor2)>>15);  
            //    wTcQ1 = 0;
            //    wTcQ2 = 0x1fff-(INT16S)(((INT32S)wTa*wFactor2)>>15)-wTc;
            //}
            //else
            //{
            //    if(wIo2<0) // swap bus voltage balance factor
            //    {
            //        wtemp = wFactor1;
            //        wFactor1 = wFactor2;
            //        wFactor2 = wtemp; 
            //    }                      
            //    wTaQ1 = (INT16S)(((INT32S)wTc*wFactor1)>>15)+wTa;
            //    wTaQ2 = 0x1fff;
            //    wTbQ1 = (INT16S)(((INT32S)wTc*wFactor1)>>15); 
            //    wTbQ2 = 0x1fff;
            //    wTcQ1 = 0;
            //    wTcQ2 = 0x1fff-(INT16S)(((INT32S)wTc*wFactor2)>>15);                                            
            //}
            //break;
        case 2:
            if(wtemp1>0) 
            {
               if(wIo1<0)
                {
               	    wtemp = wFactor1;
               	    wFactor1 = wFactor2;
               	    wFactor2 = wtemp; 
              	}  
                wTaQ1 = wTa*wFactor1+wTb+wTc; 
                wTaQ2 = 1;
                wTbQ1 = 0;
                wTbQ2 = wTa*wFactor1+wTb;
                wTcQ1 = 0;
                wTcQ2 = wTa*wFactor1; 
            }  
            else 
            {
               if(wIo2<0)
                {
                    wtemp = wFactor1;
                    wFactor1 = wFactor2;
                    wFactor2 = wtemp; 
                }   
                wTaQ1 = wTc*wFactor1+wTa+wTb; 
                wTaQ2 = 1;
                wTbQ1 = wTc*wFactor1+wTa;
                wTbQ2 = 1;
                wTcQ1 = 0;
                wTcQ2 = wTc*wFactor1;
            }
            break;
        case 3:
            if(wIo2<0)
            {
                wtemp = wFactor1;
                wFactor1 = wFactor2;
                wFactor2 = wtemp; 
            }
            wTaQ1 = wTc*wFactor1+wTa+wTb; 
            wTaQ2 = 1;
            wTbQ1 = wTc*wFactor1+wTa;
            wTbQ2 = 1;
            wTcQ1 = 0;
            wTcQ2 = wTc*wFactor1;
            break;
        case 4:
            if(wIo1<0)
            {
                wtemp = wFactor1;
                wFactor1 = wFactor2;
                wFactor2 = wtemp; 
            }
            wTaQ1 = wTa*wFactor1+wTb+wTc; //
            wTaQ2 = 1;
            wTbQ1 = 0;
            wTbQ2 = wTa*wFactor1+wTb;//
            wTcQ1 = 0;
            wTcQ2 = wTa*wFactor1;            
            break;  
        default:
            wTaQ1 =wTc*0.5+wTa*0.5+wTb;
            wTaQ2 = 1;
            wTbQ1 = wTc*0.5;
            wTbQ2 = 1-wTa*0.5;
            wTcQ1 = 0;
            wTcQ2 = 1-wTc*0.5-wTa*0.5;
            break;
    }
    
    /********<Step 5>****compute three phase switch active time (duty) for first sector ***END *******/ 
    
    /********<Step 6>****compute three phase switch active time (duty) for six sectors ****START *******/   
    switch(wSector)
    {
        case 1:
            wPwmTaQ1 = wTaQ1;
            wPwmTaQ2 = wTaQ2;
            wPwmTbQ1 = wTbQ1;
            wPwmTbQ2 = wTbQ2;
            wPwmTcQ1 = wTcQ1;
            wPwmTcQ2 = wTcQ2;
            break;
        case 2:
            wPwmTaQ1 = 1-wTbQ2;
            wPwmTaQ2 = 1-wTbQ1;
            wPwmTbQ1 = 1-wTcQ2;
            wPwmTbQ2 = 1-wTcQ1;
            wPwmTcQ1 = 1-wTaQ2;
            wPwmTcQ2 = 1-wTaQ1;
            break;    
        case 3:
            wPwmTaQ1 = wTcQ1;
            wPwmTaQ2 = wTcQ2;
            wPwmTbQ1 = wTaQ1;
            wPwmTbQ2 = wTaQ2;
            wPwmTcQ1 = wTbQ1;
            wPwmTcQ2 = wTbQ2;
            break;
        case 4:
            wPwmTaQ1 = 1-wTaQ2;
            wPwmTaQ2 = 1-wTaQ1;
            wPwmTbQ1 = 1-wTbQ2;
            wPwmTbQ2 = 1-wTbQ1;
            wPwmTcQ1 = 1-wTcQ2;
            wPwmTcQ2 = 1-wTcQ1;
            break;
        case 5:                
            wPwmTaQ1 = wTbQ1;
            wPwmTaQ2 = wTbQ2;
            wPwmTbQ1 = wTcQ1;
            wPwmTbQ2 = wTcQ2;
            wPwmTcQ1 = wTaQ1;
            wPwmTcQ2 = wTaQ2;
            break;
        case 6:
            wPwmTaQ1 = 1-wTcQ2;
            wPwmTaQ2 = 1-wTcQ1;
            wPwmTbQ1 = 1-wTaQ2;
            wPwmTbQ2 = 1-wTaQ1;
            wPwmTcQ1 = 1-wTbQ2;
            wPwmTcQ2 = 1-wTbQ1;
            break;
        default:                  
            wPwmTaQ1 = wTaQ1;
            wPwmTaQ2 = wTaQ2;
            wPwmTbQ1 = wTbQ1;
            wPwmTbQ2 = wTbQ2;
            wPwmTcQ1 = wTcQ1;
            wPwmTcQ2 = wTcQ2;
            break;                                               
    }    
    
     //converte the duty of Q2 (switch 2) to the duty of Q4(switch 4). there are Q1,Q2,Q3,Q4 switchs in every bridge branch 
    wPwmTaQ2 = 1 - wPwmTaQ2;
    wPwmTbQ2 = 1 - wPwmTbQ2;
    wPwmTcQ2 = 1 - wPwmTcQ2;
    
  
        if(Ia > 0)
        {
            if(Ia > 0) wPwmTaQ1 = wPwmTaQ1 + cPwmDeadTime;
            else wPwmTaQ1 = wPwmTaQ1 - cPwmDeadTime;
        }
        else 
        {
            if(Ia > 0) wPwmTaQ2 = wPwmTaQ2 - cPwmDeadTime;
            else wPwmTaQ2 = wPwmTaQ2 + cPwmDeadTime;	
        }
        
        if(Ib > 0)
        {
            if(Ib > 0) wPwmTbQ1 = wPwmTbQ1 + cPwmDeadTime;
            else wPwmTbQ1 = wPwmTbQ1 - cPwmDeadTime;
        }
        else 
        {
            if(Ib > 0) wPwmTbQ2 = wPwmTbQ2 - cPwmDeadTime;
            else wPwmTbQ2 = wPwmTbQ2 + cPwmDeadTime;	
        }
        
        if(Ic > 0)
        {
            if(Ic > 0) wPwmTcQ1 = wPwmTcQ1 + cPwmDeadTime;
            else wPwmTcQ1 = wPwmTcQ1 - cPwmDeadTime;
        }
        else 
        {
            if(Ic > 0) wPwmTcQ2 = wPwmTcQ2 - cPwmDeadTime;
            else wPwmTcQ2 = wPwmTcQ2 + cPwmDeadTime;	
        }
    
    
    ////Limiter of max PWM and min PWM
    wtemp = wPwmTaQ1 + wPwmTaQ2 - 1;
    if(wtemp > 0)
    {
        wPwmTaQ1 = wPwmTaQ1 - wtemp*0.5;
        wPwmTaQ2 = wPwmTaQ2 - wtemp*0.5;
    }
    
    wtemp = wPwmTbQ1 + wPwmTbQ2 - 1;
    if(wtemp > 0)
    {
        wPwmTbQ1 = wPwmTbQ1 - wtemp*0.5;
        wPwmTbQ2 = wPwmTbQ2 - wtemp*0.5;
    }
    
    wtemp =wPwmTcQ1 + wPwmTcQ2 - 1;
    if(wtemp > 0)
    {
        wPwmTcQ1 = wPwmTcQ1 - wtemp*0.5;
        wPwmTcQ2 = wPwmTcQ2 - wtemp*0.5;
    }
                                          
    //Limiter of max PWM and min PWM
    if(wPwmTaQ1>1)
    {
        wPwmTaQ1 = 1;
    }
    if(wPwmTaQ1<cMinPwm)
    {
    	wPwmTaQ1 = 0; 
    }  
    
    if(wPwmTaQ2>cMaxPwm)
    {
    	wPwmTaQ2 = cMaxPwm;
    }
    if(wPwmTaQ2<cMinPwm)
    {
    	wPwmTaQ2 = 0; 
    }
      
    if(wPwmTbQ1>cMaxPwm)
    {
    	wPwmTbQ1 = cMaxPwm;
    }
    if(wPwmTbQ1<cMinPwm)
    {
    	wPwmTbQ1 = 0;
    } 
    if(wPwmTbQ2>cMaxPwm)
    {
    	wPwmTbQ2 = cMaxPwm;
    }
    if(wPwmTbQ2<cMinPwm)
    {
    	wPwmTbQ2 = 0;  
    }              
                       
    if(wPwmTcQ1>cMaxPwm)
    {
    	wPwmTcQ1 = cMaxPwm;
    }
    if(wPwmTcQ1<cMinPwm)
    { 
    	wPwmTcQ1 = 0;
    }                                
      
    if(wPwmTcQ2>cMaxPwm)
    {
    	wPwmTcQ2 = cMaxPwm;
    }
    if(wPwmTcQ2<cMinPwm)
    {
    	wPwmTcQ2 = 0; 
    }
   
    /********<Step 6>****compute three phase switch active time (duty) for six sectors ****END *******/              
    
    /********<Step 7>****compute modulate based on duty****Start *******/         
   /* wPwmTaQ1 = wPwmTaQ1*uwInvCtrPWMPrd;
    //if(wPwmTaQ1<10)
    //{
    //    wPwmTaQ1 = 0;
    //} 
   // wPwmTaQ2 = wPwmTaQ2*uwInvCtrPWMPrd;
    //if(wPwmTaQ2<10)
    //{
    //    wPwmTaQ2 = 0;
    //}
    wPwmTbQ1 = wPwmTbQ1*uwInvCtrPWMPrd;
    //if(wPwmTbQ1<10)
    //{            
    //    wPwmTbQ1 = 0;
    //}
    wPwmTbQ2 = wPwmTbQ2*uwInvCtrPWMPrd;
    //if(wPwmTbQ2<10)
    //{    
    //    wPwmTbQ2 = 0;
    //}
    wPwmTcQ1 = wPwmTcQ1*uwInvCtrPWMPrd;
    //if(wPwmTcQ1<10)
    //{ 
    //    wPwmTcQ1 = 0;
    //}
    wPwmTcQ2 = wPwmTcQ2*uwInvCtrPWMPrd;  
    //if(wPwmTcQ2<10)
    //{
    //    wPwmTcQ2 = 0;
    //} 
    /********<Step 7>****compute modulate based on duty****End*******/
   // y[0] = wPwmTaQ1;
    
    y[0]=  wPwmTaQ1;
    y[1] = wPwmTaQ2;
    y[2] = wPwmTbQ1;
    y[3] = wPwmTbQ2;
    y[4] = wPwmTcQ1;
    y[5] = wPwmTcQ2;
   
   
    
    
}



#define MDL_DERIVATIVES
/* Function: mdlDerivatives =================================================
 * Abstract:
 *      xdot = Ax + Bu
 */
static void mdlDerivatives(SimStruct *S)
{
    real_T            *dx   = ssGetdX(S);
    real_T            *x    = ssGetContStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);

    /* xdot=Ax+Bu */
   
   // dx[0]=A[0][0]*x[0]+A[0][1]*x[1]+B[0][0]*U(0)+B[0][1]*U(1);
   // dx[1]=A[1][0]*x[0]+A[1][1]*x[1]+B[1][0]*U(0)+B[1][1]*U(1);
    
    
}



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    No termination needed, but we are required to have this routine.
 */
static void mdlTerminate(SimStruct *S)
{
    UNUSED_ARG(S); /* unused input argument */
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
