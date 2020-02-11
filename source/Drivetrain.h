#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

class Drivetrain {
public:
	//Drivetrain class idealized as a writeable X velocity and Theta steering angle.  This idealization may change
	//to make control easier.
	Drivetrain();
	void setPercentOutputX(float percent);  //Set drivetrain duty cycle in percentage from -1 to 1
	void setTheta(float angle);  //Set steering angle theta
};

#endif /* DRIVETRAIN_H_ */
