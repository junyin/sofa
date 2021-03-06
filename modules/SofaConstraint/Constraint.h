/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CONSTRAINT_CONSTRAINT_H
#define SOFA_CONSTRAINT_CONSTRAINT_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_CONSTRAINT
#  define SOFA_CONSTRAINT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_CONSTRAINT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

namespace sofa
{

namespace simulation
{

namespace tree
{

/// @brief Initialize the SofaSimulationTree library, as well as its
/// dependencies: SofaSimulationCommon, SofaCore, SofaDefaultType, SofaHelper.
void SOFA_SIMULATION_TREE_API init();

} // namespace tree

} // namespace simulation

} // namespace sofa

#endif
