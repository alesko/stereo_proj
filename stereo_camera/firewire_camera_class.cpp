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
  if( !capture_ ) 
    {        
      fprintf( stderr, "ERROR: capture is NULL \n" );     
      //getchar();     
      //return -1;   
    }                   
  will_display_ = TRUE;     
}

FireWireCamera::~FireWireCamera()
{
  cvReleaseCapture(&capture_);
}

bool FireWireCamera::Initialize(void)
{
    XYZ_ y = {0.0,1.0,0.0};
    camera_.vp.x = 0;
    camera_.vp.y = 0;
    camera_.vp.z = camera.focallength;
    camera_.vd.x = 0;
    camera_.vd.y = 0;
    camera_.vd.z = -1;
    camera_.vu = y;
}
                                   
bool FireWireCamera::QueryFrame(void)
{

  
  if( will_display_ == TRUE )                 
  {
    //g_image1 = cvQueryFrame( G_camera1->capture_ );
    //G_camera1->QueryFrame();
    camera_image_ = cvQueryFrame( capture_ );
    cvCvtColor(camera_image_, camera_image_, CV_BGR2RGB);      // Convert BGR to RBG 
  
    //cvCvtColor(g_image1, g_image1, CV_BGR2RGB);      // Convert BGR to RBG                    
    //g_display1 = FALSE;   
    //G_camera1->will_display_ = FALSE;
  }           
  //               else
  //               {
  //                  g_display1 = TRUE;
                    //G_camera1->will_display_ = TRUE;
  //               }
                 //glBindTexture(GL_TEXTURE_2D, texture_left);
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
  
  
  
  return TRUE;                      
}

/*bool FireWireCamera::WillDisplay(void)
{
     return will_display_;
}

void FireWireCamera::ToggleWillDisplay()
{
     will_display_ = !will_display_;
}*/
