#ifndef FILTER_H
#define FILTER_H


class Filter {

public:

    double beta; 
    double x_filt;

    Filter(double beta_);

    virtual ~Filter();

    void Init(double x_filt_);
    double LowPass(double x);

};

#endif /* FILTER_H */

