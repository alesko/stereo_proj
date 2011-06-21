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

#ifndef STEREODISPLAY_H_
#define STEREODISPLAY_H_

#include "firewire_camera_class.h"
#include "winhandle.h"	// To use the keys
#include <windows.h>

//#define TWOPI 6.283185307179586476925287
//#define M_PI 3.1415926535897932384626433832795
//efine DTOR 0.0174532925
//efine RTOD 57.2957795
#define ESC 27

#define TRUE  1
#define FALSE 0

class StereoDisplay {
  public:
    StereoDisplay(GL_Window* window, Keys* keys,int w_width, int w_height);
    ~StereoDisplay();
    void Draw();
    //bool Update (DWORD milliseconds, FILE* fh);
    bool Update(void);
    //bool SetRecording(bool state);
    void SetFileName(void);    
    
    LPCSTR RecFileName_;
    
  private:

    bool recording_;
    FireWireCamera *camera0_;
    FireWireCamera *camera1_;
    GL_Window*	window_;
    Keys*		keys_;
             
    bool show_right_;
    CvVideoWriter* writer_;
    IplImage* logpolarframe_;
      
};


#endif
