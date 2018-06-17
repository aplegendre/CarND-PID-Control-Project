# **PID Control**
## Writeup for Term 2, Project 4


---

**PID Control Project**

The goal of this project is to implement a PID controller that will allow the car in the simulator to go around track 1 without going off the road. There is no minimum speed, so the controller has been set to target just below 40mph


## [Rubric](https://review.udacity.com/#!/rubrics/824/view) Points
### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. Your code should compile.

My code should be able to be compiled with `cmake` and `make` without issue. I did not make any changes to CMakeLists.txt.

### Implementation

#### 1. The PID procedure follows what was taught in the lessons.

I implemented a standard PID controller as taught in the lessons. It includes parameters for proportial, integral, and derivative control. I also implemented a version of twiddle as recommended in the lessons for hyperparameter tuning. The twiddle algorithm is commented out in this submission.

The same PID controller was also used to control the throttle input, but the I and D parameters were set to zero. This meant that the throttle controller was simply a proportional controller, but its purpose was simply to make sure that the vehicle steering response was somewhat stable.

### Reflection
#### 1. Describe the effect each of the PID components had in your implementation.

Each of the PID components was implemented as discussed in the lessons. All of them were multiplied by their respective errors and then subtracted from the control output, which meant that they would steer toward the planned path as expected.

The P component is the proportional control parameter. This parameter is multiplied directly by the cross-track error and then subtracted from the control output. This means that larger cross-track errors will result in larger steering values towards the central path.

The I component is the intregral control parameter. This parameter corrects for biases in the car's steering system. The parameter is multiplied by the sum of all cross-track errors and then subtracted from the control output. This means that over time if there is a tendency for the car to steer at an offset from the path, then additional steering input will be provided to reduce that offset. It can also be used to intentionally introduce a bias, like staying on the left of the track in this counter-clockwise track.

The D component is the derivative control parameter. This parameter is multiplied by the difference in cross-track error from time-step to time-step. It should actually also be divided by the difference in time between control events, but the assumption that I made is that each message is provided at a single time step. This appears to be a poor assumption, since the performance of the control changes based on resolution changes in the simulator. The purpose of this parameter is to dampen the oscillation around the planned path that would occur if only proportional control were used. It will heavily correct the steering if there is a fast change in error, as there is around a sharp turn, and it will reduce the steering input if the car is returning to the center line. The D component was particularly important in making sure that the car didn't oscillate out of control and off the track.

#### 2. Describe how the final hyperparameters were chosen.

My process for tuning the parameters was to first start with a manual tuning. Due to the way the simulator track is laid out, it would not have been feasible to run an efficient parameter optimization unless the car could first pass a few turns on the track. Otherwise, most solutions would have arbitrary error depending on whether the car collided with an off-track object or not, which would impact the performance of the optimizer.

To get a first pass tuning, I started with only the P-parameter while the other two were set to zero. I adjusted it until the steering input was just below saturation on either side of the track. This is because at the edge of the track, I figured I'd want maximum steering to avoid falling off.

Next, I adjusted the D-parameter to minimize oscillations and increased it until I could make it around a few turns. Then finally, I provided a small positive value to the I term. I chose a positive value so that the car would bias towards steering left, since it was largely a counter-clockwise track.

Once I had initial parameters, I used a twiddle algorithm as recommended in the lessons. It would allow enough time for the car to complete the track at least once as long as it didn't go off track. It would then reset the simulator and move to the next paramter tuning step. Twiddle basically checks one control step up and one step down for each paramter independently. If the error is improved, the control step is increased. If the error is not improved, then the control step is decreased.

The twiddle algorithm allowed me to get slightly smoother performance than I did with manual tuning. Unfortunately, the track completion time was somewhat slow in the simulator, so I wasn't able to fully optimize the controller.

Particularly disappointing was when I discovered that the simulator did not always respond to the controller in the same way. I had actually tuned my car for about 60mph on the smallest resolution (for better performance on my laptop) for a couple hours (automatically) and was very pleased with the results. When I tried them again on a larger resolution, the car immediately crashed. I then needed to re-tune everything at a resolution that I thought would be more likely used by the reviewer. Unfortunately, I struggled to optimize at the same speeds as before and only got to about 40mph.

### Simulation

#### 1. The vehicle must successfully drive a lap around the track.

The vehicle successfully drove around the track on my laptop with the simulator set to `1400 x 1050` and `Good` graphics quality. I noticed that the optimal parameters were a function of resolution and also saw a similar comment on the Udacity SDC-ND Slack channels that others noticed a dependency on hardware. I started to implement a timing check in case the message or framerate changed between my machine and the reviewer's, but decided to submit with my optimal parameters as a first pass instead. If the vehicle does not pass on the reviewer's machine, then I ask that the test resolution and graphics settings be shared.