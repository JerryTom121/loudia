/*                                                         
** Copyright (C) 2008 Ricard Marxer <email@ricardmarxer.com.com>
**                                                                  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or   
** (at your option) any later version.                                 
**                                                                     
** This program is distributed in the hope that it will be useful,     
** but WITHOUT ANY WARRANTY; without even the implied warranty of      
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
** GNU General Public License for more details.                        
**                                                                     
** You should have received a copy of the GNU General Public License   
** along with this program; if not, write to the Free Software         
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
*/                                                                          

#include "Typedefs.h"
#include "Debug.h"

#include "OnsetComplex.h"
#include "Window.h"
#include "FFT.h"
#include "ODFComplex.h"

#include "Utils.h"

using namespace std;
using namespace Eigen;

OnsetComplex::OnsetComplex(int frameLength, int fftLength, Window::WindowType windowType, bool zeroPhase) : 
  _frameLength( frameLength ),
  _fftLength( fftLength ),
  _zeroPhase( zeroPhase ),
  _windowType( windowType ),
  _window( frameLength, windowType ), 
  _fft( fftLength, zeroPhase ),
  _odf( fftLength )
{
  
  DEBUG("OnsetComplex: Constructor frameLength: " << frameLength << 
        ", fftLength: " << fftLength);
   
  setup();
}

OnsetComplex::~OnsetComplex() {
  // TODO: Here we should free the buffers
  // but I don't know how to do that with MatrixXR and MatrixXR
  // I'm sure Nico will...
}


void OnsetComplex::setup() {
  // Prepare the buffers
  DEBUG("OnsetComplex: Setting up...");

  _window.setup();
  _fft.setup();
  _odf.setup();

  reset();

  DEBUG("OnsetComplex: Finished set up...");
}


void OnsetComplex::process(const MatrixXR& samples, MatrixXR* odfValue) {
  DEBUG("OnsetComplex: Processing windowed");
 
  _window.process(samples, &_windowed);

  _fft.process(_windowed, &_ffted);
  
  _odf.process(_ffted, odfValue);
    
  DEBUG("OnsetComplex: Finished Processing");
}

void OnsetComplex::reset() {
  // Initial values
  _window.reset();
  _fft.reset();
  _odf.reset();
}