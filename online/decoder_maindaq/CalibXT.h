#ifndef __CALIB_XT_H__
#define __CALIB_XT_H__
#include <fun4all/SubsysReco.h>
class CalibParamXT;
class CalibParamInTimeTaiwan;

class CalibXT: public SubsysReco {
 public:
  CalibXT(const std::string &name = "CalibXT");
  virtual ~CalibXT();
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:
  CalibParamXT* m_cal_xt;
  CalibParamInTimeTaiwan* m_cal_int;
};

#endif /* __CALIB_XT_H__ */
