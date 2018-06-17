#include "PID.h"
#include <algorithm>
#include <iostream>
#include <time.h>
using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	Kp_ = Kp;
	Ki_ = Ki;
	Kd_ = Kd;
	p_error_ = 0;
	d_error_ = 0;
	i_error_ = 0;
	sq_error = 0;
	n = 0;

	best_error = std::numeric_limits<double>::infinity();
	dp[0] = 0.1;
	dp[1] = 0.5;
	dp[2] = 0.00001;
	param_index = 0;
	twiddle_down = true;

	//time(&timer);

	return;
}

void PID::UpdateError(double cte) {
	//time_t now;
	//time(&now);
	//double seconds = difftime(now, timer);
	//time(&timer);
	d_error_ = (cte - p_error_);
	p_error_ = cte;
	i_error_ += cte;
	return;
}

double PID::UpdateSteering(double cte) {
	UpdateError(cte);
	double steer_value;
	steer_value = -Kp_ * p_error_ - Kd_ * d_error_ - Ki_ * i_error_;
	steer_value = max(steer_value, -1.0);
	steer_value = min(steer_value, 1.0);
	cout << "Steering Value in PID: " << steer_value << endl;
	return steer_value;
}

double PID::TotalError(double cte) {
	sq_error += cte*cte;
	n += 1;
	return sqrt(sq_error) / n;
}

void PID::Twiddle(double rmse) {
	n = 0;
	sq_error = 0;
	double p[] = { Kp_, Kd_, Ki_ };
	if (rmse < best_error) {
		best_error = rmse;
		dp[param_index] *= 1.1;
		param_index++;
		if (param_index == 3) { param_index = 0; }
		twiddle_down = true;
		p[param_index] += dp[param_index];
	}
	else {
		if (twiddle_down) {
			p[param_index] -= 2.0 * dp[param_index];
			twiddle_down = false;
		} else {
			p[param_index] += dp[param_index];
			dp[param_index] *= 0.9;
				
			param_index++;
			if (param_index == 3) { param_index = 0; }
			twiddle_down = true;

			p[param_index] += dp[param_index];
		}
	}

	Kp_ = p[0];
	Kd_ = p[1];
	Ki_ = p[2];
}

