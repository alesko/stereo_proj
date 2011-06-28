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
//#include <stdio.h>
//#include <string.h>
//#include <time.h>
//#include <iostream>

#include <windows.h>	         								// Header File For Windows
//#include <math.h>
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "winhandle.h"												// Header File For NeHeGL
#include "displaytools.h"


#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

/*FireWireCamera *G_camera0;
FireWireCamera *G_camera1;
GL_Window*	g_window;
Keys*		g_keys;*/
//long g_debug_counter;

//CAMERA  camera,camera_new;
//bool g_show_right = TRUE;


StereoDisplay::StereoDisplay(GL_Window* window, Keys* keys, int w_width, int w_height)
{
    recording_ = false;
    camera0_ = new FireWireCamera(1);
    camera1_ = new FireWireCamera(0);  
  
	window_	= window;
	keys_		= keys;
    
    camera0_->Initialize(w_width, w_height);
    camera1_->Initialize(w_width, w_height);

    camera1_->QueryFrame();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    camera0_->QueryFrame();	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

 
	//return TRUE;												// Return TRUE (Initialization Successful)
}

StereoDisplay::~StereoDisplay(void)									// Any User DeInitialization Goes Here
{
    if( true == recording_  )
      cvReleaseVideoWriter(&writer_);
    delete camera0_;
    delete camera1_;
}
    
//void StereoDisplay::SetFileName(LPSTR filename)
bool StereoDisplay::SetFileName(void)
{
     int isColor = 1;
     double fps = cvGetCaptureProperty( camera0_->capture_, CV_CAP_PROP_FPS);
     CvSize size = cvSize( (int)cvGetCaptureProperty( camera0_->capture_, CV_CAP_PROP_FRAME_WIDTH),
                           (int)cvGetCaptureProperty( camera0_->capture_, CV_CAP_PROP_FRAME_HEIGHT));
                           //http://www.xvidmovies.com/codec/ 
                           // divx.com
     //writer_ = cvCreateVideoWriter( filename, CV_FOURCC_DEFAULT, fps, size, isColor );                // XP Codec Pack 2.5.1
     //writer_ = cvCreateVideoWriter( filename, CV_FOURCC('X','V','I','D'), fps, size );                // XP Codec Pack 2.5.1
     //writer_ = cvCreateVideoWriter( "mytest.divx", CV_FOURCC('D','I','V','X'), fps, size );                // XP Codec Pack 2.5.1
     //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('U','2','6','3'), fps, size );                // XP Codec Pack 2.5.1
     //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('D','I','V','3'), fps, size );                // XP Codec Pack 2.5.1
     writer_ = cvCreateVideoWriter( "flv1.avi", CV_FOURCC('F','L','V','1'), fps, size );                // XP Codec Pack 2.5.1  
     if( NULL == writer_ )
     {
       return false;
     }      
     else
     {   
       logpolarframe_ = cvCreateImage(size, IPL_DEPTH_8U,3);
       recording_ = true;
       return true;
     }
     
}
    
/*bool  StereoDisplay::SetRecording(bool state)
{
      recording_ = state;
      return recording_;
}*/

bool StereoDisplay::Update(void)
{
	if (keys_->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
 		TerminateApplication (window_);						// Terminate The Program
	}

	if (keys_->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (window_);							// Toggle Fullscreen Mode
	}

    return TRUE;
        
}

void StereoDisplay::Draw()												// Draw Our Scene
{  

    IplImage* camera_image;
    GLfloat z=-20.0;
    if( show_right_ == TRUE)
      camera_image = camera1_->QueryFrame();
    else
      camera_image = camera0_->QueryFrame();

    show_right_ = !show_right_;

    glLoadIdentity();										// Reset The Modelview Matrix
    glBegin(GL_QUADS);										// Begin drawing the image texture
	   // Front Face
	   glTexCoord2f(1.0f, 1.0f); glVertex3f( 11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 1.0f); glVertex3f(-11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 0.0f); glVertex3f(-11.0f, -8.3f, z);
	   glTexCoord2f(1.0f, 0.0f); glVertex3f( 11.0f, -8.3f, z);
    glEnd();												// Done drawing texture
	 
    glFlush ();													// Flush The GL Rendering Pipeline
    
    if( true == recording_  )
    {
        cvLogPolar( camera_image, logpolarframe_, 
                    cvPoint2D32f(camera_image->width/2,camera_image->height/2), 
                    40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
        cvWriteFrame( writer_, logpolarframe_);
    }
   
}

        
/*
BOOL Initialize (GL_Window* window, Keys* keys, int w_width, int w_height)
{
    g_debug_counter=0;  
    G_camera0 = new FireWireCamera(1);
    G_camera1 = new FireWireCamera(0);  
  
	g_window	= window;
	g_keys		= keys;
	
    
    G_camera0->Initialize(w_width, w_height);
    G_camera1->Initialize(w_width, w_height);

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



void Deinitialize (void)	
//StereoDisplay::~StereoDisplay(void)									// Any User DeInitialization Goes Here
{
    delete G_camera0;
    delete G_camera1;
}



bool Update (DWORD milliseconds) //, FILE* fh)								// Perform Motion Updates Here
{
    g_debug_counter++;
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
        Deinitialize();
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_debug_counter > 1000)					// Is ESC Being Pressed?
	{
        Deinitialize();
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

    return TRUE;
        
}



void Draw()
{  

    GLfloat z=-20.0;
    if( show_right == TRUE)
      G_camera1->QueryFrame();
    else
      G_camera0->QueryFrame();

    show_right = !show_right;

    glLoadIdentity();										// Reset The Modelview Matrix
    glBegin(GL_QUADS);										// Begin drawing the image texture
	   // Front Face
	   glTexCoord2f(1.0f, 1.0f); glVertex3f( 11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 1.0f); glVertex3f(-11.0f,  8.3f, z);
	   glTexCoord2f(0.0f, 0.0f); glVertex3f(-11.0f, -8.3f, z);
	   glTexCoord2f(1.0f, 0.0f); glVertex3f( 11.0f, -8.3f, z);
    glEnd();												// Done drawing texture
	 
    glFlush ();													// Flush The GL Rendering Pipeline
   
}

*/
