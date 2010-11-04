/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GPU_CUDA_CUDAMECHANICALOBJECT_H
#define SOFA_GPU_CUDA_CUDAMECHANICALOBJECT_H

#include "CudaTypes.h"
#include <sofa/component/container/MechanicalObject.h>
#include <sofa/gpu/cuda/CudaTypesBase.h>

namespace sofa
{

namespace gpu
{

namespace cuda
{

template<class DataTypes>
class CudaKernelsMechanicalObject;

} // namespace cuda

} // namespace gpu

namespace component
{

namespace container
{

template<class TCoord, class TDeriv, class TReal>
class MechanicalObjectInternalData< gpu::cuda::CudaVectorTypes<TCoord,TDeriv,TReal> >
{
public:
    typedef gpu::cuda::CudaVectorTypes<TCoord,TDeriv,TReal> DataTypes;
    typedef MechanicalObject<DataTypes> Main;
    typedef core::VecId VecId;
    typedef core::ConstVecId ConstVecId;
    typedef typename Main::VMultiOp VMultiOp;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;


    typedef gpu::cuda::CudaKernelsMechanicalObject<DataTypes> Kernels;

    /// Temporary storate for dot product operation
    VecDeriv tmpdot;

    template<class T>
    class PrefetchOp : public T
    {
    public:
        int id; ///< ID in multi-operation, or -1 if inactive
        static helper::vector < Main* >& objects()
        {
            static helper::vector < Main* > v;
            return v;
        }
        PrefetchOp() : id(-1) {}
    };

    struct VDot
    {
        ConstVecId a;
        ConstVecId b;
        int size;
        double result;
    };
    PrefetchOp<VDot> preVDot;

    struct VOp
    {
        VecId v;
        ConstVecId a;
        ConstVecId b;
        double f;
        int size;
    };
    PrefetchOp< helper::vector<VOp> > preVOp;

    struct VResetForce
    {
        int size;
    };
    PrefetchOp< VResetForce > preVResetForce;

    static void accumulateForce(Main* m, bool prefetch = false);
    static void addDxToCollisionModel(Main* m, bool prefetch = false);
    static void vAlloc(Main* m, VecId v);
    static void vOp(Main* m, VecId v, ConstVecId a, ConstVecId b, double f, bool prefetch = false);
    static void vMultiOp(Main* m, const VMultiOp& ops, bool prefetch = false);
    static double vDot(Main* m, ConstVecId a, ConstVecId b, bool prefetch = false);
    static void resetForce(Main* m, bool prefetch = false);

    //loadInBaseVector
    static void copyToBaseVector(Main* m,defaulttype::BaseVector * dest, ConstVecId src, unsigned int &offset);
    //loadInCudaBaseVector
    static void copyToCudaBaseVector(Main* m,sofa::gpu::cuda::CudaBaseVector<Real> * dest, ConstVecId src, unsigned int &offset);

    //addBaseVectorToState
    static void addFromBaseVectorSameSize(Main* m, VecId dest, const defaulttype::BaseVector *src, unsigned int &offset);
    //addCudaBaseVectorToState
    static void addFromCudaBaseVectorSameSize(Main* m, VecId dest, const sofa::gpu::cuda::CudaBaseVector<Real> *src, unsigned int &offset);
};

// I know using macros is bad design but this is the only way not to repeat the code for all CUDA types
#define CudaMechanicalObject_DeclMethods(T) \
    template<> inline bool MechanicalObject< T >::canPrefetch() const; \
    template<> inline void MechanicalObject< T >::accumulateForce(const core::ExecParams* params); \
    template<> inline void MechanicalObject< T >::vOp(core::VecId v, core::ConstVecId a, core::ConstVecId b, double f, const core::ExecParams* params); \
    template<> inline void MechanicalObject< T >::vMultiOp(const VMultiOp& ops, const core::ExecParams* params); \
    template<> inline double MechanicalObject< T >::vDot(core::ConstVecId a, core::ConstVecId b, const core::ExecParams* params); \
    template<> inline void MechanicalObject< T >::resetForce(const core::ExecParams* params); \
    template<> inline void MechanicalObject< T >::addDxToCollisionModel(); \
	template<> inline void MechanicalObject< T >::copyToBaseVector(defaulttype::BaseVector * dest, core::ConstVecId src, unsigned int &offset); \
    template<> inline void MechanicalObject< T >::addFromBaseVectorSameSize(core::VecId dest, const defaulttype::BaseVector *src, unsigned int &offset);

CudaMechanicalObject_DeclMethods(gpu::cuda::CudaVec3fTypes);
CudaMechanicalObject_DeclMethods(gpu::cuda::CudaVec3f1Types);

#ifdef SOFA_GPU_CUDA_DOUBLE

CudaMechanicalObject_DeclMethods(gpu::cuda::CudaVec3dTypes);
CudaMechanicalObject_DeclMethods(gpu::cuda::CudaVec3d1Types);

#endif // SOFA_GPU_CUDA_DOUBLE

#undef CudaMechanicalObject_DeclMethods

}

} // namespace component

} // namespace sofa

#endif
