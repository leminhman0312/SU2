/*!
 * \file CHeatFVMVariable.cpp
 * \brief Definition of the solution fields.
 * \author F. Palacios, T. Economon
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

#include "../../include/variables/CHeatFVMVariable.hpp"


CHeatFVMVariable::CHeatFVMVariable(su2double heat, unsigned long npoint, unsigned long ndim, unsigned long nvar, CConfig *config)
  : CVariable(npoint, ndim, nvar, config), Gradient_Reconstruction{config->GetReconstructionGradientRequired() ? Gradient_Aux : Gradient} {

  bool low_fidelity = false;
  bool dual_time = ((config->GetTime_Marching() == DT_STEPPING_1ST) ||
                    (config->GetTime_Marching() == DT_STEPPING_2ND));

  /*--- Initialization of heat variable ---*/

  Solution = heat;
  Solution_Old = heat;

  /*--- Allocate residual structures ---*/

  Res_TruncError.resize(nPoint,nVar) = su2double(0.0);

  /*--- Only for residual smoothing (multigrid) ---*/

  for (unsigned long iMesh = 0; iMesh <= config->GetnMGLevels(); iMesh++) {
    if ((config->GetMG_CorrecSmooth(iMesh) > 0) || low_fidelity) {
      Residual_Sum.resize(nPoint,nVar);
      Residual_Old.resize(nPoint,nVar);
      break;
    }
  }

  /*--- Allocate and initialize solution for dual time strategy ---*/
  if (dual_time) {
    Solution_time_n  = heat;
    Solution_time_n1 = heat;
  }

  /*--- Gradient related fields ---*/
  Gradient.resize(nPoint,nVar,nDim,0.0);
  
  if (config->GetReconstructionGradientRequired()) {
    Gradient_Aux.resize(nPoint,nVar,nDim,0.0);
  }
  
  if (config->GetLeastSquaresRequired()) {
    Rmatrix.resize(nPoint,nDim,nDim,0.0);
  }
    
  if (config->GetKind_ConvNumScheme_Heat() == SPACE_CENTERED)
    Undivided_Laplacian.resize(nPoint,nVar);

  Max_Lambda_Inv.resize(nPoint);
  Max_Lambda_Visc.resize(nPoint);
  Delta_Time.resize(nPoint);

  if (config->GetMultizone_Problem())
    Set_BGSSolution_k();
}
