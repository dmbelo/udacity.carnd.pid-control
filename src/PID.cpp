#include "PID.h"

using namespace std;


PID::PID(double Kp_, double Ki_, double Kd_) {

    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;

    is_initialized = false;

}

PID::~PID() {}

void PID::Init(double cte) {
    
    p_error = cte;
    i_error = 0;
    d_error = 0;

    is_initialized = true;

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

