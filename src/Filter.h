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

#ifndef FILTER_H
#define FILTER_H

#include "Typedefs.h"
#include "Debug.h"

class Filter {
protected:
  // Internal parameters
  int _channels; 
  int _length; 

  // Internal variables
  MatrixXR _ina;
  MatrixXR _inb;

  MatrixXR _a;
  MatrixXR _b;

  MatrixXR _z;
  MatrixXR _samples;

  void setupState();
  void setupCoeffs();

public:
  Filter();

  Filter(int channels);

  Filter(const MatrixXR& b, const MatrixXR& a, int channels);

  ~Filter();

  void setup();

  void process(const MatrixXR& samples, MatrixXR* output);

  void reset();

  void setA(const MatrixXR& a);
  void setB(const MatrixXR& b);

  void a(MatrixXR* a);
  void b(MatrixXR* b);

  int channels() const;

  int length() const;

};

#endif  /* FILTER_H */