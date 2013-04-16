/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2012 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.0.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#ifndef __PLUMED_multicolvar_StoreColvarVessel_h
#define __PLUMED_multicolvar_StoreColvarVessel_h

#include <string>
#include <cstring>
#include <vector>
#include "vesselbase/Vessel.h"
#include "tools/DynamicList.h"

namespace PLMD {
namespace multicolvar{

class MultiColvarBase;

class StoreColvarVessel : public vesselbase::Vessel {
private:
  unsigned bufsize;
  std::vector<unsigned> start;
  MultiColvarBase* mycolv;
  std::vector< DynamicList<unsigned> > active_atoms;
protected:
/// Are the weights differentiable
  bool diffweight;
/// Get the number of stored colvar values
  unsigned getNumberOfStoredColvars() const ;
/// Get the ith value in the vessel
  double getValue( const unsigned& ) const ;  
/// Get the weight of the ith quantity in the vessel
  double getWeight( const unsigned& ) const ;
/// Add the derivatives from the value
  void addDerivatives( const unsigned& ival, double& pref, Value* value_out );
/// Add the derivatives from the weight of the value
  void addWeightDerivatives( const unsigned& ival, double& pref, Value* value_out );
public:
  static void registerKeywords( Keywords& keys );
/// Constructor
  StoreColvarVessel( const vesselbase::VesselOptions& );
/// Return the number of terms
  unsigned getNumberOfTerms(){ return 2; }
/// This does the resizing of the buffer
  void resize();
/// This makes sure things further down the chain are resized
  virtual void local_resizing()=0;
/// This makes sure all values are stored
  bool calculate();
/// Makes sure the dynamic lists are gathered
  void finish();
/// Do the calculation
  virtual void performCalculationUsingAllValues()=0;
};

inline
double StoreColvarVessel::getValue( const unsigned& ival ) const {
  plumed_dbg_assert( ival<start.size()-1 );
  return getBufferElement( start[ival] );
}

inline
double StoreColvarVessel::getWeight( const unsigned& ival ) const {
  plumed_dbg_assert( ival<start.size()-1 );
  return getBufferElement( bufsize + start[ival] );
}

inline
unsigned StoreColvarVessel::getNumberOfStoredColvars() const {
  return start.size()-1;
}

}
}
#endif