#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

void initDrivetrain();  //Initialize Drivetrain
void setPercentOutputX(int percent);  //Set drivetrain duty cycle in percentage from -1 to 1
void setTheta(int angle);  //Set steering angle theta
void setDirection(char forward);

#endif /* DRIVETRAIN_H_ */
