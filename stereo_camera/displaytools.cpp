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

FireWireCamera *G_camera0;
FireWireCamera *G_camera1;
GL_Window*	g_window;
Keys*		g_keys;


//CAMERA  camera,camera_new;
bool show_right = TRUE;
        

BOOL Initialize (GL_Window* window, Keys* keys, int w_width, int w_height)
//StereoDisplay::StereoDisplay(GL_Window* window, Keys* keys, int w_width, int w_height)
{
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
//bool StereoDisplay::Update(void)
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

void Draw()
//void StereoDisplay::Draw()												// Draw Our Scene
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

