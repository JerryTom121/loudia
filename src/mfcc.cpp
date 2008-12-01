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

#include <Eigen/Core>
#include <Eigen/Array>
#include <iostream>
#include <cmath>

#include "mfcc.h"
#include "melbands.h"
#include "dct.h"

#include "typedefs.h"
#include "debug.h"

using namespace std;

// import most common Eigen types 
USING_PART_OF_NAMESPACE_EIGEN

MFCC::MFCC(Real lowFreq, Real highFreq, int numBands, Real samplerate, int spectrumLength, int numCoeffs, Real minSpectrum, Real power) : _melbands(lowFreq, highFreq, numBands, samplerate, spectrumLength), _dct(numBands, numCoeffs) {
  DEBUG("MFCC: Constructor lowFreq: " << lowFreq << ", highFreq: " << highFreq << ", numBands: " << numBands << ", samplerate: "<< samplerate << ", spectrumLength: " << spectrumLength << ", numCoeffs: " << numCoeffs);
  
  _lowFreq = lowFreq;
  _highFreq = highFreq;
  _numBands = numBands;
  _samplerate = samplerate;
  _spectrumLength = spectrumLength;

  _numCoeffs = numCoeffs;

  _minSpectrum = minSpectrum;
  _power = power;
}

MFCC::~MFCC() {
  // TODO: Here we should free the buffers
  // but I don't know how to do that with MatrixXR and MatrixXR
  // I'm sure Nico will...
}


void MFCC::setup(){
  // Prepare the buffers
  DEBUG("MFCC: Setting up...");

  _melbands.setup();
  _dct.setup();
  
  reset();
  DEBUG("MFCC: Finished set up...");
}


void MFCC::process(MatrixXR spectrum, MatrixXR* mfccCoeffs){
  spectrum = spectrum.cwise().square();

  DEBUG("MFCC: Processing Melbands");
  _melbands.process(spectrum, &_bands);

  DEBUG("MFCC: Processing Log of bands");
  _bands = ((_bands.cwise() + _minSpectrum).cwise().log() / log(10.0)).cwise().pow(_power);
  
  DEBUG("MFCC: Processing DCT")
  _dct.process(_bands, mfccCoeffs);

  DEBUG("MFCC: Finished Processing")
}

void MFCC::reset(){
  // Initial values
  _bands.set(MatrixXR::Zero(_numBands, 1));

  _melbands.reset();
  _dct.reset();
}

int MFCC::numCoeffs() const {
  return _numCoeffs;
}

Real MFCC::lowFreq() const {
  return _lowFreq;
}

Real MFCC::highFreq() const {
  return _highFreq;
}
