#include <SDL/SDL.h>
#include <gl/gl.h>

#include <windows.h>
#include <stdio.h>

#include <cv.h>
#include <highgui.h>


/*
 PLEASE NOTE: the program will require SDL.dll which is located in
              dev-c++'s dll directory. You have to copy it to you
			  program's home directory or the path.
 */

int main(int argc, char *argv[]){

  SDL_Event event;
  
  int camera_left_ = 0;
  int camera_right_ = 1;
  bool will_display_l_ = true;
  bool will_display_r_ = true;  
  bool display_right_ = true;

  IplImage* camera_image_left_;
  IplImage* camera_image_right_;
  IplImage* logpolarframe_;
  GLuint texture_;
  CvVideoWriter* writer_;
  CvCapture* capture_left_ = cvCaptureFromCAM( camera_left_ );
  if( !capture_left_ ) 
    {        
      printf("ERROR: capture is NULL \n" );     
      exit(0);    
    }     
  else
    {
      cvSetCaptureProperty(capture_left_, CV_CAP_PROP_FPS, 30);       
      cvSetCaptureProperty(capture_left_, CV_CAP_PROP_FRAME_WIDTH, 800);      
      cvSetCaptureProperty(capture_left_, CV_CAP_PROP_FRAME_HEIGHT, 600);      
    }     
  CvCapture* capture_right_ = cvCaptureFromCAM( camera_right_ );
  if( !capture_right_ ) 
    {        
      printf("ERROR: capture is NULL \n" );     
      exit(0);    
    }     
  else
    {
      cvSetCaptureProperty(capture_right_, CV_CAP_PROP_FPS, 30);       
      cvSetCaptureProperty(capture_right_, CV_CAP_PROP_FRAME_WIDTH, 800);      
      cvSetCaptureProperty(capture_right_, CV_CAP_PROP_FRAME_HEIGHT, 600);      
    }     

  // Init the video recording
  double fps = cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FPS);
  CvSize size = cvSize( (int)cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FRAME_HEIGHT));
  //writer_ = cvCreateVideoWriter( "flv1.avi", CV_FOURCC_DEFAULT, fps, size );                // XP Codec Pack 2.5.1  
  writer_ = cvCreateVideoWriter( "default.avi", CV_FOURCC_DEFAULT, fps, size, (int)1);                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( filename, CV_FOURCC('X','V','I','D'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( "mytest.divx", CV_FOURCC('D','I','V','X'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('U','2','6','3'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('D','I','V','3'), fps, size );                // XP Codec Pack 2.5.1
  // writer_ = cvCreateVideoWriter( , CV_FOURCC('F','L','V','1'), fps, size );                // XP Codec Pack 2.5.1  
     logpolarframe_ = cvCreateImage(size, IPL_DEPTH_8U,3);

  SDL_Init(SDL_INIT_VIDEO);
  int width = 800;
  int height = 600;
  SDL_Surface* screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_NOFRAME | SDL_FULLSCREEN);

  // Some OpenGL initialization from stereoviewer
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color is black
  glClearDepth (1.0f);								// Depth Buffer Setup
  glDepthFunc (GL_LEQUAL);							// The Type Of Depth Testing (Less Or Equal)
  glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
  glShadeModel (GL_SMOOTH);							// Select Smooth Shading
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Set Perspective Calculations To Most Accurate
  glEnable( GL_TEXTURE_2D );                         // Enable texture mapping

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer

  glViewport(0, 0, (GLint)width, (GLint)height); // Set viewport
  
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();           

  glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f); // creat an orthogonal projection matrix
 
  glMatrixMode( GL_MODELVIEW );
  glDrawBuffer(GL_BACK_RIGHT);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
  glLoadIdentity();
	
  
  glMatrixMode (GL_PROJECTION);
  
  glGenTextures( 1, &texture_ );
  camera_image_left_ = cvQueryFrame( capture_left_ );
  cvCvtColor(camera_image_left_, camera_image_left_, CV_BGR2RGB);      // Convert BGR to RBG 
  glBindTexture(GL_TEXTURE_2D, texture_ );
  glTexImage2D(GL_TEXTURE_2D,        //target
               0,                    //level
               GL_RGB,               //internalFormat
               camera_image_left_->width,         //width
               camera_image_left_->height,        //height
               0,                    //border
               GL_RGB,               //format
               GL_UNSIGNED_BYTE,     //type
               camera_image_left_->imageData);    //pointer to image data  
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  int done;
  for(done = 0; !done;){

    display_right_ = !display_right_;

    if( display_right_ == false )                 
    {
      if ( will_display_l_  == true )   
      {        
        camera_image_left_ = cvQueryFrame( capture_left_ );
        cvCvtColor(camera_image_left_, camera_image_left_, CV_BGR2RGB);      // Convert BGR to RBG 
      }
      will_display_l_ = !will_display_l_;
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, texture_ );
      glTexImage2D(GL_TEXTURE_2D,        //target
                   0,                    //level
                   GL_RGB,               //internalFormat
                   camera_image_left_->width,         //width
                   camera_image_left_->height,        //height
                   0,                    //border
                   GL_RGB,               //format
                   GL_UNSIGNED_BYTE,     //type
                   camera_image_left_->imageData);    //pointer to image data 
    }
    else
    {
      if ( will_display_r_  == true )  
      {
        camera_image_right_ = cvQueryFrame( capture_right_ );
        cvCvtColor(camera_image_right_, camera_image_right_, CV_BGR2RGB);      // Convert BGR to RBG 
        cvLogPolar( camera_image_right_, logpolarframe_, 
                    cvPoint2D32f(camera_image_right_->width/2,camera_image_right_->height/2), 
                    40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
        //cvWriteFrame( writer_, logpolarframe_);
      }
      will_display_r_ = !will_display_r_;
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, texture_ );
      glTexImage2D(GL_TEXTURE_2D,        //target
                   0,                    //level
                   GL_RGB,               //internalFormat
                   camera_image_right_->width,         //width
                   camera_image_right_->height,        //height
                   0,                    //border
                   GL_RGB,               //format
                   GL_UNSIGNED_BYTE,     //type
                   camera_image_right_->imageData);    //pointer to image data 

    }
       

    glLoadIdentity();
    glBegin(GL_QUADS);										// Begin drawing the image texture
       glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  -1.0f);	   
       glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  -1.0f);
	   glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
	   glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, 1.0f);
    glEnd();

    SDL_GL_SwapBuffers();
    SDL_PollEvent(&event);
    if(event.key.keysym.sym == SDLK_ESCAPE)
      done = 1;
  }
  cvReleaseVideoWriter(&writer_);
  cvReleaseCapture(&capture_left_);
  cvReleaseCapture(&capture_right_);

  SDL_Quit();
  return(0);
}
