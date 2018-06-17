#ifndef PID_H
#define PID_H
#include <time.h>

class PID {
public:
  /*
  * Errors
  */
  double p_error_;
  double i_error_;
  double d_error_;
  double sq_error;
  double n;

  /*
  * Coefficients
  */ 
  double Kp_;
  double Ki_;
  double Kd_;
  time_t timer;

  /*
  *  Twiddle parameters
  */
  double best_error;
  double dp[3];
  int param_index;
  bool twiddle_down;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Update the steering values based on PID controller.
  */
  double UpdateSteering(double cte);
  
  /*
  * Calculate the total PID error.
  */
  double TotalError(double cte);

  /*
  * Optimize PID parameters
  */
  void Twiddle(double rmse);
};

#endif /* PID_H */
