struct SsrSPatternForward2 : public smacc::SmaccState<SsrSPatternForward2, SS>
{
  using SmaccState::SmaccState;

  typedef mpl::list<smacc::transition<EvActionSucceeded<smacc::ClMoveBaseZ, OrNavigation>, SsrSPatternRotate3>,
                    smacc::transition<EvActionAborted<smacc::ClMoveBaseZ, OrNavigation>, SsrSPatternRotate2>
                    > reactions;

  static void onDefinition()
  {
  }

  void onInitialize()
  {
    auto &superstate = this->context<SS>();

    this->configure<OrNavigation, CbNavigateForward>(SS::pitch1_lenght_meters());
    this->configure<OrTool, CbToolStart>();
  }
};