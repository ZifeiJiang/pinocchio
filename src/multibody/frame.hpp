//
// Copyright (c) 2016-2021 CNRS INRIA
//

#ifndef __pinocchio_multibody_frame_hpp__
#define __pinocchio_multibody_frame_hpp__

#include "pinocchio/spatial/se3.hpp"
#include "pinocchio/spatial/inertia.hpp"
#include "pinocchio/multibody/fwd.hpp"
#include "pinocchio/multibody/tree.hpp"

#include <string>

namespace pinocchio
{
  ///
  /// \brief Enum on the possible types of frame
  ///
  enum FrameType
  {
    OP_FRAME     = 0x1 << 0, // operational frame type
    JOINT        = 0x1 << 1, // joint frame type
    FIXED_JOINT  = 0x1 << 2, // fixed joint frame type
    BODY         = 0x1 << 3, // body frame type
    SENSOR       = 0x1 << 4  // sensor frame type
  };

  template<typename _Scalar, int _Options>
  struct traits< FrameTpl<_Scalar,_Options> >
  {
    typedef _Scalar Scalar;
    enum { Options = _Options };
  };
  
  ///
  /// \brief A Plucker coordinate frame attached to a parent joint inside a kinematic tree
  ///
  template<typename _Scalar, int _Options>
  struct FrameTpl : ModelItem< FrameTpl<_Scalar,_Options> >
  {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef FrameTpl<_Scalar, _Options> ModelItemDerived;
    typedef typename traits<ModelItemDerived>::Scalar Scalar;
    enum { Options = traits<ModelItemDerived>::Options };
    typedef ModelItem<ModelItemDerived> Base;
    
    typedef SE3Tpl<Scalar,Options> SE3;
    typedef InertiaTpl<Scalar,Options> Inertia;
    typedef pinocchio::JointIndex JointIndex;
    
    ///
    /// \brief Default constructor of a frame.
    ///
    FrameTpl()
    : Base()
    , type()
    , inertia(Inertia::Zero())
    {} // needed by std::vector
    
    ///
    /// \brief Builds a frame defined by its name, its joint parent id, its placement and its type.
    ///
    /// \param[in] name Name of the frame.
    /// \param[in] parent Index of the parent joint in the kinematic tree.
    /// \param[in] frame_placement Placement of the frame wrt the parent joint frame.
    /// \param[in] type The type of the frame, see the enum FrameType.
    /// \param[in] inertia Inertia info attached to the frame.
    ///
    FrameTpl(const std::string & name,
             const JointIndex parentJoint,
             const SE3 & frame_placement,
             const FrameType type,
             const Inertia & inertia = Inertia::Zero())
      : Base(name, parentJoint, 0, frame_placement)
      , type(type)
      , inertia(inertia)
    {}
    
    ///
    /// \brief Builds a frame defined by its name, its joint parent id, its placement and its type.
    ///
    /// \param[in] name Name of the frame.
    /// \param[in] parent Index of the parent joint in the kinematic tree.
    /// \param[in] previousFrame Index of the parent frame in the kinematic tree.
    /// \param[in] frame_placement Placement of the frame wrt the parent joint frame.
    /// \param[in] type The type of the frame, see the enum FrameType.
    /// \param[in] inertia Inertia info attached to the frame.
    ///
    FrameTpl(const std::string & name,
             const JointIndex parent_joint,
             const FrameIndex previous_frame,
             const SE3 & frame_placement,
             const FrameType type,
             const Inertia & inertia = Inertia::Zero())
      : Base(name, parent_joint, previous_frame, frame_placement)
      , type(type)
      , inertia(inertia)
    {}
    
    ///
    /// \brief Copy constructor by casting
    ///
    /// \param[in] other Frame to copy
    ///
    template<typename S2, int O2>
    explicit FrameTpl(const FrameTpl<S2,O2> & other)
    {
      *this = other.template cast<Scalar>();
    }
    
    ///
    /// \brief Equality comparison operator.
    ///
    /// \returns true if *this is equal to other.
    ///
    /// \param[in] other The frame to which the current frame is compared.
    ///
    template<typename S2, int O2>
    bool operator ==(const FrameTpl<S2,O2> & other) const
    {
      return name == other.name
      && parentJoint == other.parentJoint
      && parentFrame == other.parentFrame
      && placement == other.placement
      && type == other.type
      && inertia == other.inertia;
    }

    ///
    /// \returns true if *this is NOT equal to other.
    ///
    template<typename S2, int O2>
    bool operator != (const FrameTpl<S2,O2> & other) const
    {
      return !(*this == other);
    }

    /// \returns An expression of *this with the Scalar type casted to NewScalar.
    template<typename NewScalar>
    FrameTpl<NewScalar,Options> cast() const
    {
      typedef FrameTpl<NewScalar,Options> ReturnType;
      ReturnType res(name,
                     parentJoint,
                     parentFrame,
                     placement.template cast<NewScalar>(),
                     type,
                     inertia.template cast<NewScalar>());
      return res;
    }
    
    // data

    using Base::name;
    using Base::parentFrame;
    using Base::parentJoint;
    using Base::placement;

    /// \brief Type of the frame.
    FrameType type;
    
    /// \brief Inertia information attached to the frame.
    ///        This inertia will be appended to the inertia supported by the parent joint when calling ModelTpl::addFrame.
    ///        It won't be processed otherwise by the algorithms.
    Inertia inertia;

  }; // struct FrameTpl

  template<typename Scalar, int Options>
  inline std::ostream & operator << (std::ostream& os,
                                     const FrameTpl<Scalar,Options> & f)
  {
    os
    << "Frame name: "
    << f.name
    << " paired to (parent joint/ previous frame)"
    << "(" << f.parent << "/" << f.previousFrame << ")"
    << std::endl
    << "with relative placement wrt parent joint:\n"
    << f.placement
    << "containing inertia:\n"
    << f.inertia
    << std::endl;
    
    return os;
  }

} // namespace pinocchio

#endif // ifndef __pinocchio_multibody_frame_hpp__
