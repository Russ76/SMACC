/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018-2020
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#pragma once

#include <move_group_interface_client/cl_movegroup.h>
#include <smacc/smacc_asynchronous_client_behavior.h>
#include <future>
namespace cl_move_group_interface
{
class CbMoveEndEffectorTrajectory : public smacc::SmaccAsyncClientBehavior
{
public:
  geometry_msgs::PoseStamped targetPose;
  std::string tip_link_;
  boost::optional<std::string> group_;

  CbMoveEndEffectorTrajectory();
  CbMoveEndEffectorTrajectory(geometry_msgs::PoseStamped target_pose, std::string tip_link = "");

  virtual void onEntry() override;

  std::future<moveit::planning_interface::MoveItErrorCode> planAndExecuteAsync();

protected:
  bool moveToAbsolutePose(moveit::planning_interface::MoveGroupInterface &moveGroupInterface,
                          geometry_msgs::PoseStamped &targetObjectPose);

  ClMoveGroup *movegroupClient_;
};
}  // namespace cl_move_group_interface