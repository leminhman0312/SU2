/*!
 * \file CTurbVariable.cpp
 * \brief Definition of the solution fields.
 * \author F. Palacios, A. Bueno
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/variables/CTurbVariable.hpp"


CTurbVariable::CTurbVariable(unsigned long npoint, unsigned long ndim, unsigned long nvar, CConfig *config)
  : CVariable(npoint, ndim, nvar, config) {

  /*--- Allocate space for the harmonic balance source terms ---*/

  if (config->GetTime_Marching() == HARMONIC_BALANCE) {
    HB_Source.resize(nPoint,nVar) = su2double(0.0);
  }

  /*--- Gradient related fields ---*/

  Gradient.resize(nPoint,nVar,nDim,0.0);

  if (config->GetKind_Gradient_Method() == WEIGHTED_LEAST_SQUARES) {
    Rmatrix.resize(nPoint,nDim,nDim,0.0);
  }

  /*--- Always allocate the slope limiter, and the auxiliar
   variables (check the logic - JST with 2nd order Turb model) ---*/

  Limiter.resize(nPoint,nVar) = su2double(0.0);
  Solution_Max.resize(nPoint,nVar) = su2double(0.0);
  Solution_Min.resize(nPoint,nVar) = su2double(0.0);

  Delta_Time.resize(nPoint);

  if (config->GetError_Estimate() || config->GetKind_SU2() == SU2_MET) {
    AnisoViscGrad.resize(nPoint,nDim*nVar*nDim) = su2double(0.0);
    AnisoViscHess.resize(nPoint,3*(nDim-1)*nVar*nDim) = su2double(0.0);
  }
}