#include "filter.h"

Filter::Filter(double beta_) {

    beta = beta_;
    
}

Filter::~Filter() {} 

void Filter::Init(double x_filt_) {

    x_filt = x_filt_;

}

double Filter::LowPass(double x) {

    double output;

    x_filt = beta * x + (1 - beta) * x_filt;

    return x_filt;

}