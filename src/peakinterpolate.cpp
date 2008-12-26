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

#include <Eigen/Core>
#include <Eigen/Array>
#include <iostream>
#include <cmath>

#include "peakinterpolate.h"

#include "typedefs.h"
#include "debug.h"

using namespace std;

// import most common Eigen types 
using namespace Eigen;

PeakInterpolate::PeakInterpolate() {
  DEBUG("PEAKINTERPOLATE: Constructor");
  

  setup();
  DEBUG("PEAKINTERPOLATE: Constructed");
}

PeakInterpolate::~PeakInterpolate() {
  // TODO: Here we should free the buffers
  // but I don't know how to do that with MatrixXR and MatrixXR
  // I'm sure Nico will...
}


void PeakInterpolate::setup(){
  // Prepare the buffers
  DEBUG("PEAKINTERPOLATE: Setting up...");

  reset();
  DEBUG("PEAKINTERPOLATE: Finished set up...");
}


void PeakInterpolate::process(MatrixXR spectrum,
                              MatrixXR peakPositions, MatrixXR peakMagnitudes,
                              MatrixXR* peakPositionsInterp, MatrixXR* peakMagnitudesInterp){
  
  DEBUG("PEAKINTERPOLATE: Processing");  
  Real leftMag;
  Real rightMag;
  Real mag;
  
  (*peakPositionsInterp).resize(spectrum.rows(), peakPositions.cols());
  (*peakMagnitudesInterp).resize(spectrum.rows(), peakPositions.cols());
  
  for ( int row = 0 ; row < spectrum.rows(); row++ ) {
  
    for ( int i = 0; i < peakPositions.cols(); i++ ) {
      
      // If the position is -1 do nothing since it means it is nothing
      if( peakPositions(row, i) == -1 ){
        
        (*peakMagnitudesInterp)(row, i) = -1;
        
        (*peakPositionsInterp)(row, i) = -1;
      
      } else {
        
        // Take the center magnitude in dB
        mag = 20.0 * log10( peakMagnitudes(row, i) );

        // Take the left magnitude in dB
        if( peakPositions(row, i) <= 0 ){
          
          leftMag = 20.0 * log10( spectrum(row, (int)peakPositions(row, i) + 1) );
          
        } else {
          
          leftMag = 20.0 * log10( spectrum(row, (int)peakPositions(row, i) - 1) );
          
        }
        
        // Take the right magnitude in dB
        if( peakPositions(row, i) >= spectrum.row(row).cols() - 1 ){
          
          rightMag = 20.0 * log10( spectrum(row, (int)peakPositions(row, i) - 1) );
          
        } else {
          
          rightMag = 20.0 * log10( spectrum(row, (int)peakPositions(row, i) + 1) );
          
        }
                
        // Calculate the interpolated position in dB
        (*peakPositionsInterp)(row, i) = peakPositions(row, i) + 0.5 * (leftMag - rightMag) / (leftMag - 2.0 * mag + rightMag);

        // Calculate the interpolated magnitude in dB
        (*peakMagnitudesInterp)(row, i) = mag - 0.25 * (leftMag - rightMag) * ((*peakPositionsInterp)(row, i) - peakPositions(row, i));

      }

    }
  }
  
  DEBUG("PEAKINTERPOLATE: Finished Processing");
}

void PeakInterpolate::reset(){
  // Initial values
}

