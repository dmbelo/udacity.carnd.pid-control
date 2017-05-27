#ifndef PID_H
#define PID_H

class PID {

public:

  bool is_initialized;

  // Errors
  double p_error;
  double i_error;
  double d_error;

  // Inidividual contorl components
  double u_p;
  double u_i;
  double u_d;

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
