#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include <math.h>
#include "PID.h"
#include "utils.h"

// TODO Implement speed control
// TODO Implement twiddle at constant speed
// TODO Implement speed dependent steering gains
// TODO Implement filter on steer/throttle control

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

  // Initialize the pid object
  PID pid(0.1, 0.01, 0.001);
  Utils utils;

  double previous_time;

  // Open a file to save some results
	FILE * file;
  file = fopen ("simout.txt", "w");

  h.onMessage([&pid, &utils, &file, &previous_time](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
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
          double steer_value;
        
          if (!pid.is_initialized) {
            
            pid.Init(cte);
            steer_value = -cte * pid.Kp;
            previous_time = clock();

          }
          else {

            double current_time = clock();
            double dt = (current_time - previous_time) / CLOCKS_PER_SEC;
            previous_time = current_time;
            pid.UpdateError(cte, dt);
            steer_value = -pid.ControlActuation();            

          }        

          // steer_value = utils.SmoothSaturation(steer_value_unbounded, 1.0, 0.1);
          if (steer_value > 1.0) {
            steer_value = 1.0;
          }
          else if (steer_value < -1.0) {
            steer_value = -1.0;
          }
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          fprintf(file, "%10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f, %10.6f\n",
                  speed, angle, cte, steer_value, pid.p_error, pid.i_error, pid.d_error);

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
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

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
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
