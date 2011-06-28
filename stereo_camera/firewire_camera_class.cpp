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

#include "firewire_camera_class.h"

FireWireCamera::FireWireCamera(int camera_index)
{
  camera_index_ = camera_index;
  capture_ = cvCaptureFromCAM( camera_index_ ); 
  //capture_ = cvCreateCameraCapture( CV_CAP_FIREWIRE + camera_index_ );  
  if( !capture_ ) 
    {        
      printf("ERROR: capture is NULL \n" );     
      //getchar(); 
      exit(0);    
    }     
  else
    {
      //cvSetCaptureProperty(capture_, CV_CAP_PROP_FPS, 25); 
      //      cvSetCaptureProperty(capture_, CV_CAP_PROP_FRAME_WIDTH, 640);      
      //      cvSetCaptureProperty(capture_, CV_CAP_PROP_FRAME_HEIGHT, 480);      
      cvSetCaptureProperty(capture_, CV_CAP_PROP_FPS, 15);       
      cvSetCaptureProperty(capture_, CV_CAP_PROP_FRAME_WIDTH, 320);      
      cvSetCaptureProperty(capture_, CV_CAP_PROP_FRAME_HEIGHT, 240);      

    }              
  will_display_ = TRUE;     
}

FireWireCamera::~FireWireCamera()
{
  cvReleaseCapture(&capture_);
}

bool FireWireCamera::Initialize(int w_width, int w_height)
{
    // Set camera parameters
  	XYZ_ u = {0.0,1.0,0.0};  // Up
	XYZ_ p = {0.0,0.0,1.0};  // Position
	XYZ_ d = {0.0,0.0,0.0}; // Direction (center?)
    camera_.vp = p;
    camera_.vd = d;
    camera_.vu = u;
        
    // Some OpenGL initialization
	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth (1.0f);								// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);							// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
	glShadeModel (GL_SMOOTH);							// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Set Perspective Calculations To Most Accurate
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	
	
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
 
    glViewport (0, 0, (GLsizei)(w_width), (GLsizei)(w_height));				// Reset The Current Viewport
    glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
    glLoadIdentity ();													// Reset The Projection Matrix
    
    
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
  
    gluLookAt(camera_.vp.x, camera_.vp.y, camera_.vp.z,  // Camera position 
              camera_.vd.x, camera_.vd.y, camera_.vd.z,  // Camera view point/direction
              camera_.vu.x, camera_.vu.y, camera_.vu.z); // Camera up
	
    return TRUE;
}

bool FireWireCamera::DrawImage(void)
{    

    return TRUE;

}
                                   
IplImage* FireWireCamera::QueryFrame(void)
{
  
  if( will_display_ == TRUE )                 
  {
    camera_image_ = cvQueryFrame( capture_ );
    cvCvtColor(camera_image_, camera_image_, CV_BGR2RGB);      // Convert BGR to RBG 
  }           
  will_display_ = !will_display_; // Toggle   

  glBindTexture(GL_TEXTURE_2D, texture_ );
  glTexImage2D(GL_TEXTURE_2D,        //target
                              0,                    //level
                              GL_RGB,               //internalFormat
                              camera_image_->width,         //width
                              camera_image_->height,        //height
                              0,                    //border
                              GL_RGB,               //format
                              GL_UNSIGNED_BYTE,     //type
                              camera_image_->imageData);    //pointer to image data 
  
  
  
  return camera_image_; //TRUE;                      
}

/*bool FireWireCamera::WillDisplay(void)
{
     return will_display_;
}

void FireWireCamera::ToggleWillDisplay()
{
     will_display_ = !will_display_;
}*/
