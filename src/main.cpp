#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include <math.h>
#include "PID.h"
#include "utils.h"
#include "filter.h"

// Check for time step?
// TODO Implement twiddle at constant speed
// TODO Implement speed dependent steering gains

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  // Instantiate the pid objects
  // PID steer_pid(0.1, 0.03, 0.005); // 20 MPH
  PID steer_pid(0.1*0.85, 0.03*0.65*0, 0.005*0.65); // 50 MPH
  PID speed_pid(0.1, 0.05, 0.00);

  // Instantiate filter object
  Filter throttle_filter(0.3);
  Filter steer_filter(1);
  Filter cte_filter(1);
  throttle_filter.Init(0.0);
  steer_filter.Init(0.0);
  cte_filter.Init(0.0);

  Utils utils;

  double previous_time;

  // Open a file to save some results
	FILE * file;  

  h.onMessage([&steer_pid, &speed_pid, &utils, &file, &previous_time, &cte_filter, &steer_filter, &throttle_filter](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value, throttle_value;
          double current_time = clock();
          double dt = (current_time - previous_time) / CLOCKS_PER_SEC;
          previous_time = current_time;
          double speed_target = 50.0;
          double speed_error = speed_target - speed;

          double cte_filt = cte_filter.LowPass(cte);

          if (!steer_pid.is_initialized) {
            
            steer_pid.Init(cte_filt);
            steer_value = -cte_filt * steer_pid.Kp;
            previous_time = clock();

          }
          else {

            steer_pid.UpdateError(cte_filt, dt);
            steer_value = -steer_pid.ControlActuation();            

          }        

          if (!speed_pid.is_initialized) {

            speed_pid.Init(speed_error);
            throttle_value = speed_pid.Kp;
            
          }
          else {

            speed_pid.UpdateError(speed_error, dt);
            throttle_value = speed_pid.ControlActuation();

          }

          // steer_value = utils.SmoothSaturation(steer_value_unbounded, 1.0, 0.1);
          if (steer_value > 1.0) {

            steer_value = 1.0;

          }
          else if (steer_value < -1.0) {

            steer_value = -1.0;

          }

          if (throttle_value > 1.0) {

            throttle_value = 1.0;
            
          }
          else if (throttle_value < -1.0) {

            throttle_value = -1.0;

          }
          
          double steer_raw = steer_value;

          steer_value = steer_filter.LowPass(steer_value);
          throttle_value = throttle_filter.LowPass(throttle_value);

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << " Speed: " << speed << std::endl;

          fprintf(file, "%10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f\n",
                  speed, angle, cte, cte_filt, steer_raw, steer_value,
                  steer_pid.p_error, steer_pid.i_error, steer_pid.d_error, 
                  steer_pid.u_p, steer_pid.u_i, steer_pid.u_d);

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h, &file](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
    file = fopen ("simout.txt", "w");
  });

  h.onDisconnection([&h, &file](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
    fclose (file);
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
