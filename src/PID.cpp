#include "PID.h"

using namespace std;


PID::PID(double Kp_, double Ki_, double Kd_) {

    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;

    u_p = 0;
    u_i = 0;
    u_d = 0;

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

    u_p = p_error * Kp;
    u_i = i_error * Ki;
    u_d = d_error * Kd;

    double u = u_p + u_i + u_d;
    
    return u;

}

