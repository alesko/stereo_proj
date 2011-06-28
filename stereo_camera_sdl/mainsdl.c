#include <SDL/SDL.h>
#include <gl/gl.h>

//#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
//#include <cvcam.h>
#include <cxcore.h>

/*
 PLEASE NOTE: the program will require SDL.dll which is located in
              dev-c++'s dll directory. You have to copy it to you
			  program's home directory or the path.
 */

using namespace std;

int main(int argc, char *argv[]){

  cout << "Hello" << endl;         
  bool stereo = true;
  bool mirror = false;
  bool text = false;
  bool record = false;    
  int width = 400;
  int height = 300;


  for(int i = 1; i < argc; i++){
    if( 0 == strcmp(argv[i], "-ns") )
      stereo = false;
    if( 0 == strcmp(argv[i], "-m") )
      mirror = true;    
   if( 0 == strcmp(argv[i], "-t") )
      text = true;
   if( 0 == strcmp(argv[i], "-r") )
      record = true;    
  }   

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
      cvSetCaptureProperty(capture_left_, CV_CAP_PROP_FRAME_WIDTH, width);      
      cvSetCaptureProperty(capture_left_, CV_CAP_PROP_FRAME_HEIGHT, height);      
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
      cvSetCaptureProperty(capture_right_, CV_CAP_PROP_FRAME_WIDTH, width);      
      cvSetCaptureProperty(capture_right_, CV_CAP_PROP_FRAME_HEIGHT, height);      
    }     

  // Init the video recording
  double fps = cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FPS);
  CvSize size = cvSize( (int)cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture_right_, CV_CAP_PROP_FRAME_HEIGHT));
  //writer_ = cvCreateVideoWriter( "flv1.avi", CV_FOURCC_DEFAULT, fps, size );                // XP Codec Pack 2.5.1  
  writer_ = cvCreateVideoWriter( "default.avi", CV_FOURCC_DEFAULT, fps, size);                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( filename, CV_FOURCC('X','V','I','D'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( "mytest.divx", CV_FOURCC('D','I','V','X'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('U','2','6','3'), fps, size );                // XP Codec Pack 2.5.1
  //writer_ = cvCreateVideoWriter( file_name, CV_FOURCC('D','I','V','3'), fps, size );                // XP Codec Pack 2.5.1
  // writer_ = cvCreateVideoWriter( , CV_FOURCC('F','L','V','1'), fps, size );                // XP Codec Pack 2.5.1  
  //writer_ = cvCreateVideoWriter( "myvideo.avi", CV_FOURCC('I','2','4','0'), fps, size );                // XP Codec Pack 2.5.1  
  if( writer_ == NULL ){
    cout << "writer_ == NULL" << endl;
    exit(0);
  }
  logpolarframe_ = cvCreateImage(size, IPL_DEPTH_8U,3);

  SDL_Init(SDL_INIT_VIDEO);
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

  CvFont font;
  double hScale=1.0;
  double vScale=1.0;
  int    lineWidth=1;
  cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);


  int done = 0;
  while(done == 0){

    if( stereo == true )
      display_right_ = !display_right_;

    if( display_right_ == false )                 
    {
      if ( will_display_l_  == true )   
      {        
        camera_image_left_ = cvQueryFrame( capture_left_ );
        cvCvtColor(camera_image_left_, camera_image_left_, CV_BGR2RGB);      // Convert BGR to RBG 
        if( text )
          cvPutText (camera_image_left_,"Left camera view",cvPoint(5,camera_image_left_->height-10), &font, cvScalar(255,255,0));
      }
      if( stereo == true )
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
        if( text )
          cvPutText (camera_image_right_,"Right camera view",cvPoint(5,camera_image_right_->height-10), &font, cvScalar(255,255,0));

        cvLogPolar( camera_image_right_, logpolarframe_, 
                    cvPoint2D32f(camera_image_right_->width/2,camera_image_right_->height/2), 
                    40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
        //cvWriteFrame( writer_ , logpolarframe_);
      }
      if( stereo == true )
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
    if( mirror)
    {
      glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f,  -1.0f); // top left
	  glTexCoord2f(1.0f, 0.0f);	    glVertex2f(-1.0f, 1.0f);   // bottom left
      glTexCoord2f(0.0f, 0.0f);  glVertex2f( 1.0f, 1.0f);   // bottom right
      glTexCoord2f(0.0f, 1.0f);        glVertex2f( 1.0f,  -1.0f);	// top right
    }
    else
    {
       glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  -1.0f);	// top right
       glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  -1.0f); // top left
	   glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);   // bottom left
	   glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, 1.0f);   // bottom right
    }   
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
