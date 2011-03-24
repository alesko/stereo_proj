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

#include <GL/glut.h>

#include <stdlib.h>
#include <cv.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>

#include <windows.h>	         								// Header File For Windows
#include <math.h>
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
//#include "vfw.h"												// Header File For Video For Windows
#include "winhandle.h"												// Header File For NeHeGL
#include "displaytools.h"
#include "firewire_camera_class.h"

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

FireWireCamera *G_camera0;
FireWireCamera *G_camera1;
GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
//float		angle;												// Used For Rotation
//bool		bg=TRUE;											// Background (Default On)
//bool		bp;													// 'B' Pressed?

CAMERA  camera;
bool show_right = TRUE;
//bool g_display0 = TRUE;
//bool g_display1 = TRUE;
//IplImage* g_image0;
//IplImage* g_image1;
//GLuint texture_right,texture_left;
        

BOOL Initialize (GL_Window* window, Keys* keys)
{
    G_camera0 = new FireWireCamera(0);
    G_camera1 = new FireWireCamera(1);  
    
 
	g_window	= window;
	g_keys		= keys;

	XYZ y = {0.0,1.0,0.0};
    camera.vp.x = 0;
    camera.vp.y = 0;
    camera.vp.z = camera.focallength;
    camera.vd.x = 0;
    camera.vd.y = 0;
    camera.vd.z = -1;
    camera.vu = y;

	// Start Of User Initialization
	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping


    G_camera1->QueryFrame();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    G_camera0->QueryFrame();
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
    delete G_camera0;
    delete G_camera1;
}

bool Update (DWORD milliseconds, FILE* fh)								// Perform Motion Updates Here
{
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
        Deinitialize();
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}


	/*if ((g_keys->keyDown ['B']) && !bp)							// Is 'B' Being Pressed And Not Held?
	{
		bp=TRUE;												// Set bp To True
		bg=!bg;													// Toggle Background Off/On
	}
	if (!g_keys->keyDown['B'])									// Is 'B' Released?
		bp=FALSE;												// Set bp To False
		
	*/
    return TRUE;
        
}

void Draw (int w_width, int w_height)												// Draw Our Scene
{  
    int z=-20;
    XYZ r;
    XYZ pmin = {-2,-2,-100},pmax = {2,2,1};
    double ratio,radians,wd2,ndfl;
    double left,right,top,bottom;
   
    camera.near_ = camera.focallength / 10;
    camera.near_ = camera.focallength *0.10;

    // Misc stuff needed for the frustum 
    ratio   = camera.screenwidth / (double)camera.screenheight;
    if (camera.stereo == DUALSTEREO)
       ratio /= 2;
    radians = DTOR * camera.aperture / 2;
    wd2     = camera.near_ * tan(radians);
    ndfl    = camera.near_ / camera.focallength;
    top     =   wd2;
    bottom  = - wd2;

    /* Determine the right eye vector */
    CROSSPROD(camera.vd,camera.vu,r);
    Normalise(&r);
    r.x *= camera.eyesep / 2.0;
    r.y *= camera.eyesep / 2.0;
    r.z *= camera.eyesep / 2.0;
    
 	  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
 
      glViewport (0, 0, (GLsizei)(w_width), (GLsizei)(w_height));				// Reset The Current Viewport
	  glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	  glLoadIdentity ();													// Reset The Projection Matrix
	  if( show_right == TRUE)
      {
        left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
        right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
      }
      else
      {
        left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
        right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
      }

      GLdouble fW, fH;
      GLdouble fovY= 45.0f;
      GLdouble aspect = (GLfloat)(w_width)/(GLfloat)(w_height);
      GLdouble zNear  = 1;
      GLdouble zFar   = 100;
    
      fH = tan( fovY / 360 * M_PI ) * zNear;
      fW = fH * aspect;
      glFrustum( -fW, fW, -fH, fH, zNear, zFar );               
	
	  glMatrixMode(GL_MODELVIEW);
      glDrawBuffer(GL_BACK_RIGHT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
  

 	  if( show_right == TRUE)
      {
        gluLookAt(camera.vp.x + r.x,camera.vp.y + r.y,camera.vp.z + r.z, 
                 camera.vp.x + r.x + camera.vd.x,
                 camera.vp.y + r.y + camera.vd.y,
                 camera.vp.z + r.z + camera.vd.z,
                 camera.vu.x,camera.vu.y,camera.vu.z);
                 show_right = FALSE;
        G_camera1->QueryFrame();
     }
     else
     {
       gluLookAt(camera.vp.x - r.x,camera.vp.y - r.y,camera.vp.z - r.z,
                 camera.vp.x - r.x + camera.vd.x,
                 camera.vp.y - r.y + camera.vd.y,
                 camera.vp.z - r.z + camera.vd.z,
                 camera.vu.x,camera.vu.y,camera.vu.z);
       G_camera0->QueryFrame();
       show_right = TRUE;
     }

	 glLoadIdentity();										// Reset The Modelview Matrix
	 glBegin(GL_QUADS);										// Begin Drawing The Background (One Quad)
	   // Front Face
	   glTexCoord2f(1.0f, 1.0f); glVertex3f( 11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 1.0f); glVertex3f(-11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 0.0f); glVertex3f(-11.0f, -8.3f, z);
	   glTexCoord2f(1.0f, 0.0f); glVertex3f( 11.0f, -8.3f, z);
	 glEnd();												// Done Drawing The Background
	 
   glFlush ();													// Flush The GL Rendering Pipeline
}


/*
   Handle a window reshape/resize
*/
/*void HandleReshape(int w,int h)
{
   camera.screenwidth = w;
   camera.screenheight = h;
}*/

/*
   Rotate (ix,iy) or roll (iz) the camera about the focal point
*/
/*void RotateCamera(double ix,double iy,double iz)
{
   XYZ vp,vu,vd,right;
   XYZ newvp,newr;
   double dx,dy,dz,radius;

   vu = camera.vu;
   Normalise(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalise(&vd);
   CROSSPROD(vd,vu,right);
   Normalise(&right);

   // Handle the roll 
   if (iz != 0) {
      camera.vu.x += iz * right.x;
      camera.vu.y += iz * right.y;
      camera.vu.z += iz * right.z;
      Normalise(&camera.vu);
      return;
   }

   // Distance from the rotate point
   dx = camera.vp.x - camera.pr.x;
   dy = camera.vp.y - camera.pr.y;
   dz = camera.vp.z - camera.pr.z;
   radius = sqrt(dx*dx + dy*dy + dz*dz);

   // Determine the new view point
   newvp.x = vp.x + ix * right.x + iy * vu.x - camera.pr.x;
   newvp.y = vp.y + ix * right.y + iy * vu.y - camera.pr.y;
   newvp.z = vp.z + ix * right.z + iy * vu.z - camera.pr.z;
   Normalise(&newvp);
   camera.vp.x = camera.pr.x + radius * newvp.x;
   camera.vp.y = camera.pr.y + radius * newvp.y;
   camera.vp.z = camera.pr.z + radius * newvp.z;

   // Determine the new right vector 
   newr.x = camera.vp.x + right.x - camera.pr.x;
   newr.y = camera.vp.y + right.y - camera.pr.y;
   newr.z = camera.vp.z + right.z - camera.pr.z;
   Normalise(&newr);
   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

   camera.vd.x = camera.pr.x - camera.vp.x;
   camera.vd.y = camera.pr.y - camera.vp.y;
   camera.vd.z = camera.pr.z - camera.vp.z;
   Normalise(&camera.vd);

   // Determine the new up vector 
   CROSSPROD(newr,camera.vd,camera.vu);
   Normalise(&camera.vu);
}
*/
/*
   Translate (pan) the camera view point
*/
/*
void TranslateCamera(double ix,double iy,double iz)
{
   XYZ right;

   CROSSPROD(camera.vd,camera.vu,right);
   Normalise(&right);

   camera.vp.x += iy*camera.vu.x + ix*right.x + iz*camera.vd.x;
   camera.vp.y += iy*camera.vu.y + ix*right.y + iz*camera.vd.y;
   camera.vp.z += iy*camera.vu.z + ix*right.z + iz*camera.vd.z;

   camera.pr.x += iy*camera.vu.x + ix*right.x + iz*camera.vd.x;
   camera.pr.y += iy*camera.vu.y + ix*right.y + iz*camera.vd.y;
   camera.pr.z += iy*camera.vu.z + ix*right.z + iz*camera.vd.z;
}

*/

/*
   Move the camera to the home position 
*/
/*
void CameraHome(int position)
{
   XYZ origin = {0,0,0};
   XYZ up = {0.0,0.0,1.0},x = {1.0,0.0,0.0}, y = {0.0,1.0,0.0};

   switch (position) {
   case 0:
      camera.aperture = 60;
      camera.focallength = 6;
      camera.eyesep = camera.focallength / 25;
      camera.near_ = camera.focallength / 10;
      camera.far_ = camera.focallength * 10;
      camera.vp.x = -camera.focallength;
      camera.vp.y = 0; 
      camera.vp.z = 0;
      camera.vd.x = 1;
      camera.vd.y = 0;
      camera.vd.z = 0;
      camera.vu = up;
      break;
   case 1: // Front
      camera.vp.x = -camera.focallength;
      camera.vp.y = 0;
      camera.vp.z = 0;
      camera.vd.x = 1;
      camera.vd.y = 0;
      camera.vd.z = 0;
      camera.vu = up;
      break;
   case 2: // Back 
      camera.vp.x = camera.focallength;
      camera.vp.y = 0;
      camera.vp.z = 0;
      camera.vd.x = -1;
      camera.vd.y = 0;
      camera.vd.z = 0;
      camera.vu = up;
      break;
   case 3: // Left 
      camera.vp.x = 0;
      camera.vp.y = camera.focallength;
      camera.vp.z = 0;
      camera.vd.x = 0;
      camera.vd.y = -1;
      camera.vd.z = 0;
      camera.vu = up;
      break;
   case 4: // Right
      camera.vp.x = 0;
      camera.vp.y = -camera.focallength;
      camera.vp.z = 0;
      camera.vd.x = 0;
      camera.vd.y = 1;
      camera.vd.z = 0;
      camera.vu = up;
      break;
   case 5: // Top 
      camera.vp.x = 0;
      camera.vp.y = 0;
      camera.vp.z = camera.focallength;
      camera.vd.x = 0;
      camera.vd.y = 0;
      camera.vd.z = -1;
      camera.vu = x;
      break;
   case 6: // Bottom 
      camera.vp.x = 0;
      camera.vp.y = 0;
      camera.vp.z = -camera.focallength;
      camera.vd.x = 0;
      camera.vd.y = 0;
      camera.vd.z = 1;
      camera.vu = x;
      break;
   case 7: // Top
      camera.vp.x = 0;
      camera.vp.y = 0;
      camera.vp.z = camera.focallength;
      camera.vd.x = 0;
      camera.vd.y = 0;
      camera.vd.z = -1;
      camera.vu = y;
      break;
   }

   camera.pr = origin;
   Normalise(&camera.vd);
   Normalise(&camera.vu);
}
*/
/*
   Calculate the length of a vector
*/
/*double Modulus(XYZ p)
{
    return(sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
}
*/
/*
   Normalise a vector
*/

void Normalise(XYZ *p)
{
   double length;

   length = p->x * p->x + p->y * p->y + p->z * p->z;
   if (length > 0) {
      length = sqrt(length);
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}
