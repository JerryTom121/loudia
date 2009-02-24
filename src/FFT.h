/*                                                         
** Copyright (C) 2008 Ricard Marxer <email@ricardmarxer.com>
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

#ifndef FFT_H
#define FFT_H

#include "Typedefs.h"
#include "Debug.h"

#include <fftw3.h>

class FFT{
protected:
  int _fftSize;
  bool _zeroPhase;

  int _halfSize;
  
  Real* _in;
  fftwf_complex* _out;
  fftwf_plan _fftplan;
  

public:
  FFT(int fftSize, bool zeroPhase = true);
  ~FFT();
  
  void process(const MatrixXR& frames, MatrixXC* fft);
  
  void setup();
  void reset();
  
  int fftSize() const;
};

#endif  /* FFT_H */