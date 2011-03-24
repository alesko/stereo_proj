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

#ifndef FIREWIRE_CAMERA_CLASS_H_
#define FIREWIRE_CAMERA_CLASS_H_

#include <cv.h>
#include <highgui.h>
#include <GL/glut.h>
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>	
			
typedef struct {
	double x,y,z;
} XYZ_;

#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

typedef struct {
   XYZ_ vp;                /* View position           */
   XYZ_ vd;                /* View direction vector   */
   XYZ_ vu;                /* View up direction       */
   //XYZ_ pr;                /* Point to rotate about   */
   double focallength;    /* Focal Length along vd   */
   double aperture;       /* Camera aperture         */
   double eyesep;         /* Eye separation          */
	//double near,far;       /* Cutting plane distances */
	
   double near_;
   double far_;
   int stereo;				  /* Are we in stereo mode   */
   int screenwidth;       /* Screen dimensions       */
   int screenheight;      /*                         */
} CAMERA_;


class FireWireCamera {
      
  public:
    FireWireCamera(int camera_index);
    ~FireWireCamera(void);
    bool QueryFrame(void);
    bool Initialize(int w_width, int w_height);
    bool WillDisplay(void);
    void ToggleWillDisplay();
    bool DrawImage(void);
    
    CvCapture* capture_;    
    IplImage* camera_image_;
    GLuint texture_;    
    bool will_display_;  // Should the image be displayed ?
    
    CAMERA_ camera_;
    
    
  private:
    int camera_index_;   // Uniqe index
    
      
};

#endif
