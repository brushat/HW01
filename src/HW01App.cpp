/**
* @file HW01.cpp
* 
* @author Adam Brush
* @date 2012-08-28
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix is as long as you
* give attribution.  Commercial uses are allowed.
* 
* This project was heavily influenced by Dr. Brinkman's solution for the HW01 project:
* https://github.com/brinkmwj/HW01
* 
* This project fulfills the following requirements for homework 1:
* 
* 
*
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
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int radius, Color8u fill);
	void drawRectangle(uint8_t* pixels,int start_x, int start_y, int rect_x, int rect_y, Color8u fill);
	void gradient(uint8_t* pixels, Color8u fill);
	void clearBackground(uint8_t* pixels, int kAppWidth, int kAppHeight);
	void makeLine( uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fill);
	void tint(uint8_t* pixels, Color8u tint);
};

void HW01App::prepSettings(Settings* settings) {
	(*settings).setWindowSize(kWinWidth,kWinHeight);
	(*settings).setResizable(true);
}

void HW01App::clearBackground(uint8_t* pixels, int kAppWidth, int kAppHeight){
    for(int i = 0; i<kAppWidth*kAppHeight*3; i++){
        pixels[i] = 0;
    }
}

void HW01App::makeLine ( uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c ){ 

	 int     x, offset;       
	 float   dy, dx, y, m;


	//formula from geometry
	 dy = y1 - y0;
	 dx = x1 - x0;

	 m = dy / dx;
	 y = y0;

	for ( x = x0; x <= x1; x++ ) {
		int index = 3 * (x + (y * kSurfaceSize)); 
     
		 pixels [index]=  c.r;
		 pixels [index+1]= c.g;
		 pixels [index+2]= c.b; 
      
				  y += m;   //* Step y by slope m 
			   }			// end for loop

}
void HW01App::tint(uint8_t* pixels, Color8u color)
{	for(int y = 0; y < kWinHeight; y++){
		for(int x = 0; x < kWinWidth; x++){	
			int index = 3*(x + y*kSurfaceSize);

			pixels[index]   += color.r;	
			pixels[index+1] += color.g;	
			pixels[index+2] += color.b;		
		}	
	}
}

void HW01App::drawCircle (uint8_t* pixels, int center_x, int center_y, int radius, Color8u fill){
	for(int y = center_y - radius; y <= center_y + radius; y++)
	{
		for(int x = center_x - radius; x <= center_x + radius; x++)
		{
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;
			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(dist <= radius)
			{
					int offset = 3*(x + y*kSurfaceSize);
					pixels[offset] = fill.r;
					pixels[offset+1] = fill.g;
					pixels[offset+2] = fill.b;
			}
		}
	}
}

void HW01App::drawRectangle (uint8_t* pixels, int start_x, int start_y, int width, int height, Color8u fill)
{

	// start at the top left of where the rectangle should be in the array and change the pixels from there
	for (int y = start_y; y <= (start_y + height); y++)
	{
		for (int x = start_x; x <= (start_x + width); x++)
		{
			int offset = 3*(x + y*kSurfaceSize);
			pixels[offset] = fill.r;
			pixels[offset+1] = fill.g;
			pixels[offset+2] = fill.b;
		}
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

void HW01App::update()
{
	myPixels = (*mySurface_).getData();
	
	brightnessR1_ = (float)rand()/(float)RAND_MAX;
	if(brightnessR1_<0.0f)
		brightnessR1_ = 1.0f;
	brightnessG1_ = brightnessG1_ + 0.01f;
	if(brightnessG1_>1.0f)
		brightnessG1_ = 0.0f;
	brightnessB1_ = brightnessB1_ - 0.01f;
	if(brightnessB1_<0.0f)
		brightnessB1_ = 1.0f;
	///
	brightnessR2_ = (float)rand()/(float)RAND_MAX;
	if(brightnessR2_>1.0f)
		brightnessR2_ = 0.0f;
	brightnessG2_ = brightnessG2_ + 0.01f;
	if(brightnessG2_>1.0f)
		brightnessG2_ = 0.0f;
	brightnessB2_ = brightnessB2_ + 0.2f;
	if(brightnessB2_>1.0f)
		brightnessB2_ = 0.0f;
	
			drawRectangle(myPixels, 0, 201, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawRectangle(myPixels, 401, 201, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawRectangle(myPixels, 201, 0, 200,200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawRectangle(myPixels, 601, 0, 199, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawRectangle(myPixels, 201, 401, 200, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawRectangle(myPixels, 601, 401, 199, 200, Color(brightnessR1_,brightnessB1_,brightnessG1_));
			drawCircle(myPixels, 100, 100, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			drawCircle(myPixels, 300, 300, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			drawCircle(myPixels, 500, 100, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			drawCircle(myPixels, 700, 300, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			drawCircle(myPixels, 500, 500, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			drawCircle(myPixels, 100, 500, 50, Color(brightnessR2_,brightnessB2_,brightnessG2_));
			//drawLine(myPixels, 100, 1000, 1000, 500, Color8u(200,50,50));

	// tint reacts interestingly because I am using the random number generator so it keeps redrawing in what looks like different colors
	tint(myPixels, Color8u(100,1,1));

	if(frameNumber_ == 0){
		writeImage("brushat.png",*mySurface_);
		//We do this here, instead of setup, because we don't want to count the writeImage time in our estimate
		app_start_time_ = boost::posix_time::microsec_clock::local_time();
	}
	frameNumber_++;
}

void HW01App::draw()
{
	
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HW01App, RendererGl )
