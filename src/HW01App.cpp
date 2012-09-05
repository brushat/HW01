/**
* @file HW01.cpp
* 
* @author Adam Brush
* @date 2012-08-28
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix it as long as you
* give attribution.  Commercial uses are allowed.
* 
* This project was heavily influenced by Dr. Brinkman's solution for the HW01 project:
* https://github.com/brinkmwj/HW01
* Specifically his work on creating the surface, transparency, and saving the image
* 
* This project fulfills the following requirements for homework 1:
* 
* A1, A2, A3, and A6 as well as C, D, E2, and E5.
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepSettings(Settings* settings);

private:
	static const int kWinWidth = 800;
	static const int kWinHeight = 600;
	static const int kSurfaceSize=1024;
	/// Brightness of the screen. Must be between 0.0f (black) and 1.0f (white), inclusive.
	float brightnessR1_;
	float brightnessG1_;
	float brightnessB1_;
	float brightnessR2_;
	float brightnessG2_;
	float brightnessB2_;
	
	int frameNumber_;
	boost::posix_time::ptime app_start_time_;
	Surface* mySurface_;
	uint8_t* myPixels;
	
	/**
	* Draw a rectangle
	* 
	* This method draws a rectangle based on the (x, y) coordinate of the top left corner of the rectangle,
	* the side lengths of x and y and the color of the rectangle.  It assumes the user will not put negatives
	* in for the coordinates or side lengths
	* 
	* It satisfies the rectangle requirement
	*/
	void drawRectangle(uint8_t* pixels,int start_x, int start_y, int rect_x, int rect_y, Color8u fill);
	
	/**
	* Draw a circle
	* 
	* This method will draw a circle based on the center point, radius, and color of the circle
	* 
	* It satisfies the draw circle method
	*/
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int radius, Color8u fill);
	
	/**
	* 
	* Draw a line segment
	* 
	* This method draws a line segment based on the (x, y) coordinate of the start of the line and the length
	* and color of the line
	* 
	* It satisfies the line segment requirement but doesn't have full functionality based on the fact that
	* you can't make diagonal lines on the screen.
	*/
	void drawLineSegment(uint8_t* pixels, int x, int y, int length, Color8u fill);
	
	/**
	* Clear the screen
	* 
	* This method will clear the entire screen to black.
	* 
	* Doesn't satisfy any requirements but did help me to initially implement the update functionality
	*/
	void clearBackground(uint8_t* pixels, int kAppWidth, int kAppHeight);
	
	/**
	* Adds a tint to the entire surface
	* 
	* This method adds a tint to the entire surface by accessing the pixel array and tinting it with
	* the specified color.
	* 
	* Satisfies the tint requirement
	*/
	void tint(uint8_t* pixels, Color8u tint);
	
	/**
	* Copies a rectangular portion of the screen from one area to another
	* 
	* This method copies a rectangular region from one area of the screen to another.
	* 
	* Satisfies the copy requirement - NOT FUNCTIONAL YET
	*/
	void copyRectangle(uint8_t* pixels, int x, int y, int copyX, int copyY, int width, int height);
};

void HW01App::prepSettings(Settings* settings) {
	(*settings).setWindowSize(kWinWidth,kWinHeight);
	(*settings).setResizable(true);
}


void HW01App::drawRectangle (uint8_t* pixels, int start_x, int start_y, int width, int height, Color8u fill)
{

	// start at the top left of where the rectangle should be in the array and change the pixels from there
	for (int y = start_y; y <= (start_y + height); y++){
		// Change all of the x for the corresponding y
		for (int x = start_x; x <= (start_x + width); x++){
			// row major order
			int index = 3*(x + y*kSurfaceSize);

			pixels[index] = fill.r;
			pixels[index+1] = fill.g;
			pixels[index+2] = fill.b;
		}
	}
}


void HW01App::drawCircle (uint8_t* pixels, int center_x, int center_y, int radius, Color8u fill){
	
	for(int y = center_y - radius; y <= center_y + radius; y++){
		
		for(int x = center_x - radius; x <= center_x + radius; x++){
			
			// distance formula for two points
			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			
			if(dist <= radius){
					int index = 3*(x + y*kSurfaceSize);
					// This is where I used some of Dr. Brinkmans code to make my circles transparent looking
					// Satisfies the transparency requirement
					pixels[index] = pixels[index]/2 + fill.r/2;
					pixels[index+1] = pixels[index+1]/2 + fill.g/2;
					pixels[index+2] = pixels[index+2]/2 + fill.b/2;
			}
		}
	}
}


void HW01App::drawLineSegment(uint8_t* pixels, int x, int y, int length, Color8u fill) {
	for (int i = 0; i <= length; i++) {
		int index = 3*(x+(y*kSurfaceSize));

		pixels[index] = fill.r;
		pixels[index+1] = fill.g;
		pixels[index+2] = fill.b;
		x++;
	}
}


void HW01App::clearBackground(uint8_t* pixels, int kAppWidth, int kAppHeight){
    for(int i = 0; i<kAppWidth*kAppHeight*3; i++){
		// set everything in the pixel array to black
        pixels[i] = 0;
    }
}


void HW01App::tint(uint8_t* pixels, Color8u color){
	for(int y = 0; y < kWinHeight; y++){
		for(int x = 0; x < kWinWidth; x++){	

			int index = 3*(x + y*kSurfaceSize);

			pixels[index]   += color.r;	
			pixels[index+1] += color.g;	
			pixels[index+2] += color.b;		
		}	
	}
}


void HW01App::copyRectangle(uint8_t* pixels, int x, int y, int newX, int newY, int width, int height){
	for(int i = x; i <= width; i++){
		for(int j = y; j <= height; j++){
			int index1 = 3*(x+y*kSurfaceSize);
			int index2 = 3*(newX + newY*kSurfaceSize);
			pixels[index2] = pixels[index1];
			pixels[index2+1] = pixels[index1+1];
			pixels[index2+2] = pixels[index1+2];
			newY++;
		}
		newX++;
	}
}
void HW01App::setup()
{
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);
	frameNumber_ = 0;
}


void HW01App::mouseDown( MouseEvent event )
{

}


void HW01App::update(){
	myPixels = (*mySurface_).getData();
	
	// generates a random color
	// used random number generator help from http://www.cplusplus.com/forum/beginner/7445/
	brightnessR1_ = (float)rand()/(float)RAND_MAX;
	if(brightnessR1_<0.0f)
		brightnessR1_ = 1.0f;
	brightnessG1_ = brightnessG1_ + 0.01f;
	if(brightnessG1_>1.0f)
		brightnessG1_ = 0.0f;
	brightnessB1_ = brightnessB1_ - 0.01f;
	if(brightnessB1_<0.0f)
		brightnessB1_ = 1.0f;
	// generates a random color
	brightnessR2_ = (float)rand()/(float)RAND_MAX;
	if(brightnessR2_>1.0f)
		brightnessR2_ = 0.0f;
	brightnessG2_ = brightnessG2_ + 0.01f;
	if(brightnessG2_>1.0f)
		brightnessG2_ = 0.0f;
	brightnessB2_ = brightnessB2_ + 0.2f;
	if(brightnessB2_>1.0f)
		brightnessB2_ = 0.0f;
	
	
	// Draw the crazy checker board
	// Satisfies the draw a picture/art requirement
	drawRectangle(myPixels, 0, 201, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	drawRectangle(myPixels, 401, 201, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	drawRectangle(myPixels, 201, 0, 200,200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	drawRectangle(myPixels, 601, 0, 199, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	drawRectangle(myPixels, 201, 401, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	//copyRectangle(myPixels,201,0,601,0,199,200);
	drawRectangle(myPixels, 601, 401, 199, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
	drawCircle(myPixels, 100, 100, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawCircle(myPixels, 300, 300, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawCircle(myPixels, 500, 100, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawCircle(myPixels, 700, 300, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawCircle(myPixels, 500, 500, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawCircle(myPixels, 100, 500, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
	drawLineSegment(myPixels, 100, 100, 400, Color8u(200,50,50));
	drawLineSegment(myPixels, 300, 300, 400, Color8u(200,50,50));
	drawLineSegment(myPixels, 100, 500, 400, Color8u(200,50,50));

	// tint reacts interestingly because I am using the random number generator so it keeps redrawing in what looks like different colors
	// eventhough tint is not being fed any of the random colors
	tint(myPixels, Color8u(100,1,1));

	if(frameNumber_ == 0){
		// Same as Dr. Brinkman's code for saving
		// Satisfies the save an image requirement
		writeImage("crazycheckerboard1f.png",*mySurface_);
		app_start_time_ = boost::posix_time::microsec_clock::local_time();
	}
	if(frameNumber_ == 50){
		// Same as Dr. Brinkman's code for saving
		// Satisfies the save an image requirement
		writeImage("crazycheckerboard50f.png",*mySurface_);
		app_start_time_ = boost::posix_time::microsec_clock::local_time();
	}
	// Saved 2 images to show the changing colors on the screen as the frames update
	frameNumber_++;
}


void HW01App::draw()
{
	
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HW01App, RendererGl )
