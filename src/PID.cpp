#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {

    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    
    p_error = 0;
    i_error = 0;
    d_error = 0;

}

void PID::UpdateError(double cte, double dt) {
    
    i_error += (cte + p_error) / 2.0 * dt;
    d_error = (cte - p_error) / dt;
    p_error = cte;

}

double PID::ControlActuation() {

    double u = p_error * Kp + i_error * Ki + d_error * Kd;
    
    return u;

}

