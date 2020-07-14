#pragma once

#include <smacc/component.h>
#include <moveit_z_client/cl_movegroup.h>
#include <geometry_msgs/Vector3.h>

namespace sm_moveit_4
{
    namespace cl_moveit_z_client
    {
        // Adds two simetric collision virtual walls for the moveit planning
        class CpConstraintLateralWorkspace :  public smacc::ISmaccComponent, public smacc::ISmaccUpdatable
        {
        private:
            moveit::planning_interface::PlanningSceneInterface* planningSceneInterface_;
            ClMoveGroup* movegroupclient_;

            std::string referenceFrame_;
            float lateralDistance_;
            geometry_msgs::Vector3 offset_;
            geometry_msgs::Vector3 size_;
            bool enabled_= true;
            bool alreadyRemoved_ = true;

        public:            

            void enable()
            {
                enabled_ = true;
                alreadyRemoved_ = false;
                this->update();
            }

            void disable()
            {
                enabled_ = false;
                this->update();
            }

            CpConstraintLateralWorkspace(std::string referenceFrame, float lateralDistance,  geometry_msgs::Vector3 size, geometry_msgs::Vector3 offset = geometry_msgs::Vector3())
            {
                referenceFrame_ = referenceFrame;
                size_ = size;
                offset_ = offset;
                lateralDistance_ = lateralDistance;
                this->setUpdatePeriod(ros::Duration(10.0)); // do not update too much frequenfly since we have a relative collision
            }

            virtual void onInitialize() override
            {
                this->requiresClient(movegroupclient_);
                planningSceneInterface_ = &movegroupclient_->planningSceneInterface;
            }

            virtual void update()
            {
                if(enabled_)
                {
                    auto time = ros::Time::now();
                    std::vector<moveit_msgs::CollisionObject> collisionObjects;
                    createVirtualCollisionWalls(collisionObjects, time, moveit_msgs::CollisionObject::ADD);
                    this->planningSceneInterface_->applyCollisionObjects(collisionObjects);
                }
                else if(alreadyRemoved_)
                {
                    auto time = ros::Time::now();
                    std::vector<moveit_msgs::CollisionObject> collisionObjects;

                    createVirtualCollisionWalls(collisionObjects, time, moveit_msgs::CollisionObject::REMOVE);
                    this->planningSceneInterface_->applyCollisionObjects(collisionObjects);
                    alreadyRemoved_ = false;
                }                
            }

            void createCollisionBox(float x, float y, float z, float xl, float yl, float zl, std::string id, std::string frameid, moveit_msgs::CollisionObject &collision, const ros::Time& time, int addOrRemove)
            {
                collision.operation = addOrRemove;
                collision.id = id;

                collision.primitives.resize(1);
                collision.primitives[0].type = collision.primitives[0].BOX;
                collision.primitives[0].dimensions.resize(3);

                collision.primitives[0].dimensions[0] = xl;
                collision.primitives[0].dimensions[1] = yl;
                collision.primitives[0].dimensions[2] = zl;

                collision.primitive_poses.resize(1);
                collision.primitive_poses[0].position.x = x;
                collision.primitive_poses[0].position.y = y;
                collision.primitive_poses[0].position.z = z;
                collision.primitive_poses[0].orientation.w = 1.0;

                collision.header.frame_id = frameid;
                collision.header.stamp = time;
            }

            void createVirtualCollisionWalls(std::vector<moveit_msgs::CollisionObject> &collisions, const ros::Time& time,  int addOrRemove)
            {
                moveit_msgs::CollisionObject box;
                createCollisionBox(offset_.x, offset_.y - lateralDistance_, offset_.z, size_.x, size_.y , size_.z, "right", referenceFrame_, box, time, addOrRemove);
                collisions.push_back(box);

                createCollisionBox(offset_.x, offset_.y + lateralDistance_, offset_.z, size_.x, size_.y , size_.z, "left", referenceFrame_, box, time, addOrRemove);
                collisions.push_back(box);
            }
        };

    } // namespace cl_perception_system

} // namespace sm_moveit_4