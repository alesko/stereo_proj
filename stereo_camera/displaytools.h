/*********************************************************************
*
* Software License Agreement (BSD License)
*
* Copyright (c) 2011, Alexander Skoglund, Karolinska Institute
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of the Karolinska Institute nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*********************************************************************/

#ifndef DISPLAYTOOLS_H_
#define DISPLAYTOOLS_H_

typedef struct {
	double x,y,z;
} XYZ;

#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

typedef struct {
   XYZ vp;                /* View position           */
   XYZ vd;                /* View direction vector   */
   XYZ vu;                /* View up direction       */
   XYZ pr;                /* Point to rotate about   */
   double focallength;    /* Focal Length along vd   */
   double aperture;       /* Camera aperture         */
   double eyesep;         /* Eye separation          */
	//double near,far;       /* Cutting plane distances */
	
 double near_;
 double far_;
	int stereo;				  /* Are we in stereo mode   */
   int screenwidth;       /* Screen dimensions       */
   int screenheight;      /*                         */
} CAMERA;

/*
typedef struct {
	int debug;             // Debug mode              
	int record;            // Movie recording mode    
	int windowdump;        // Image recording modes   
	int fullscreen;        // Game mode or not        
	double targetfps;      // Target frame rate       
} OPTIONS;
typedef struct {
	int button;
	int shift;
	int mouseh;
	int mousev;
} INTERFACESTATE;*/

#define TWOPI 6.283185307179586476925287
//#define M_PI 3.1415926535897932384626433832795
#define DTOR 0.0174532925
#define RTOD 57.2957795
#define ESC 27

#define TRUE  1
#define FALSE 0

#define NOSTEREO     0
#define ACTIVESTEREO 1
#define DUALSTEREO   2

//void CreateMenus(void);
//void CreateEnvironment(void);
//void CreateGeometry(void);

//void HandleDisplay(void);
//void HandleKeyboard(unsigned char,int,int);
//void HandleSpecialKeyboard(int,int,int);
//void HandleMouse(int,int,int,int);
//void HandleReshape(int,int);
//void HandleMouseMotion(int,int);
//void HandlePassiveMotion(int,int);
//void HandleVisibility(int);
void HandleIdle(void);

//void GiveUsage(char *);
//void RotateCamera(double,double,double);
//void TranslateCamera(double,double,double);
//void CameraHome(int);

//void HandleMainMenu(int);
//void HandleCameraMenu(int);

//double Modulus(XYZ);
void Normalise(XYZ *);
//double GetRunTime(void);
//int Dump2TGA(int,int,int);
//void WriteTGA(FILE *,unsigned char *,int,int);

//void OpenAVI(LPCSTR szFile);										// Opens An AVI File (szFile)
//void GrabAVIFrame(int frame);
//void CloseAVI(void);
//void flipIt(void* buffer);

#endif
