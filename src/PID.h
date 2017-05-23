#ifndef PID_H
#define PID_H

class PID {

public:

  bool is_initialized;

  // Errors
  double p_error;
  double i_error;
  double d_error;

  // Coefficients 
  double Kp;
  double Ki;
  double Kd;

  // Constructor
  PID(double Kp_, double Ki_, double Kd_);

  // Destructor
  virtual ~PID();

  // Initialize PID
  void Init(double cte);

  // Update the PID error variables given cross track error.
  void UpdateError(double cte, double dt);

  // Calculate control
  double ControlActuation();

};

#endif /* PID_H */
