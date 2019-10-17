#pragma once
#include <memory>
#include <map>
#include <functional>
#include <vector>
#include <list>
#include <regex>

namespace smacc
{
class ISmaccState;
class SmaccStateMachineInfo;
class SmaccStateInfo;
class TypeInfo;

//---------------------------------------------
struct StateBehaviorInfoEntry
{
    std::function<void(smacc::ISmaccState *)> factoryFunction;
    const std::type_info *behaviorType;
    const std::type_info *orthogonalType;
};
//---------------------------------------------
struct SmaccTransitionInfo
{
    SmaccTransitionInfo()
    {
        eventType= nullptr;
        eventSourceType= nullptr;
    }

    int index;
    std::shared_ptr<SmaccStateInfo> sourceState;
    std::shared_ptr<SmaccStateInfo> destinyState;
    
    std::string transitionTag;
    std::shared_ptr<smacc::TypeInfo> eventType;
    std::shared_ptr<smacc::TypeInfo> eventSourceType;
    std::shared_ptr<smacc::TypeInfo> eventObjectTag;

    std::string getSourceTypeName();
};
//---------------------------------------------
struct SmaccLogicUnitInfo
{
    std::shared_ptr<SmaccStateInfo> ownerState;
    std::function<void(smacc::ISmaccState *)> factoryFunction;
    const std::type_info* logicUnitType;
};

enum class SmaccStateType{SUPERSTATE = 2, STATE = 1, SUPERSTATE_ROUTINE = 1};
//---------------------------------------------
class SmaccStateInfo : public std::enable_shared_from_this<SmaccStateInfo>
{
public:
    static std::map<const std::type_info *, std::vector<StateBehaviorInfoEntry>> staticBehaviorInfo;
    static std::map<const std::type_info *, std::vector<SmaccLogicUnitInfo>> logicUnitsInfo;

    bool active_;
    std::string fullStateName;
    std::string demangledStateName;

    std::shared_ptr<SmaccStateMachineInfo> stateMachine_;
    std::shared_ptr<SmaccStateInfo> parentState_;
    std::vector<SmaccTransitionInfo> transitions_;

    std::vector<std::shared_ptr<SmaccStateInfo>> children_;
    int depth_;
    const std::type_info* tid_;

    SmaccStateInfo(const std::type_info* tid, std::shared_ptr<SmaccStateInfo> parentState, std::shared_ptr<SmaccStateMachineInfo> stateMachineInfo);

    SmaccStateType getStateLevel();

    inline int depth() const { return depth_; }

    void getAncestors(std::list<std::shared_ptr<SmaccStateInfo>> &ancestorsList);

    std::string getFullPath();

    template <typename StateType>
    std::shared_ptr<SmaccStateInfo> createChildState();

    template <typename EvType>
    void declareTransition(std::shared_ptr<SmaccStateInfo> &dstState, std::string transitionTag );

    template<typename EvSource, template<typename> typename EvType >
    void declareTransition(std::shared_ptr<SmaccStateInfo> &dstState, std::string transitionTag);

    const std::string &toShortName() const;
};
} // namespace smacc