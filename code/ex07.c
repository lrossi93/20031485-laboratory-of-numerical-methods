#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

/*
	Prototypes
*/
double* setRange();
void setSeed();
size_t setN();
double getRandomDoubleFromRange(double min, double max);
double f(double x, double* y_max);


//==============================================================================
//==============================================================================
//==============================================================================
/*
	Main
*/
int main(){
	//Variables
	double* range = setRange();

	double 	x_min = *range,
				x_max = *(range + 1),
				y_min = 0,
				y_max = 0,
				base = x_max - x_min,
				height;

	size_t 	n = setN(),
				tot = 0,
				accepted = 0;
	FILE	*fp,
			*fp_above,
			*fp_below;

	char	filename[50],
			filename_above[10] = "above.txt",
			filename_below[10] = "below.txt";

	// creation of a well formed filename
	sprintf(filename, "points_a_%.3lf_b_%.3lf_n_%ld.txt", x_min, x_max, n);

	// set seed for random number generation
	setSeed();

	//store all generated points into n couples (x, y)
	double point[n][2];

	// open file to store generated couples
	fp = fopen(filename, "w");
	fp_above = fopen(filename_above, "w");
	fp_below = fopen(filename_below, "w");

	for(size_t i = 0; i < n; ++i){
		point[i][0] = getRandomDoubleFromRange(x_min, x_max);
		double f_x = f(point[i][0], &y_max);
		point[i][1] = getRandomDoubleFromRange(y_min, y_max);

		// write extracted point into file
		fprintf(fp, "%lf %lf\n", point[i][0], point[i][1]);

		//uncomment the next line for checking the extracted values
		//printf("x_rand: %lf\t\tf(x): %lf\t\ty_rand: %lf\n", point[i][0], f_x, point[i][1]);
		if(point[i][1] <= f_x){
			//below
			accepted++;
			fprintf(fp_below, "%lf %lf\n", point[i][0], point[i][1]);
		}
		else{
			//above
			fprintf(fp_above, "%lf %lf\n", point[i][0], point[i][1]);
		}
		tot++;
	}

	fclose(fp);
	fclose(fp_above);
	fclose(fp_below);

	printf("max(f): %lf\n", y_max);
	printf("accepted points: %lu\t total points: %lu\n", accepted, tot);
	height = y_max;

	//calculate rectangular area
	double area = base * height;

	// generated/total points ratio
	double ratio = ((double)accepted) / tot;

	// integral estimation
	double integral = ratio * area;

	// print results
	printf("rectangle area: %lf\n", area);
	printf("estimated ratio: %lf\n", ratio);
	printf("estimated integral: %lf\n", integral);

	return 0;
}
//==============================================================================
//==============================================================================
//==============================================================================


	/*
		Function implementations
	*/

/*
	Set integration range
*/
double* setRange(){
	double a = -1.0, b = -1.0;
	static double range[2];

	puts("Choose a range [a, b] not larger than (0, 10) where a < b.");

	while(a <= 0 || a >= 10){
		printf("Choose a: ");
		scanf("%lf", &a);

		if(a <= 0)
			puts("It must be a > 0!");
		if(a >= 10)
			puts("It must be a < 10!");
	}

	while(b < a || b >= 10){
		printf("Insert b: ");
		scanf("%lf", &b);

		if(a >= b)
			puts("It must be a < b!");
		if(b >= 10)
			puts("It must be b < 10!");
	}

	range[0] = a;
	range[1] = b;

	return range;
}

/*
	Set seed for random number generation from current time, so that the seed is
	always different
*/
void setSeed(){
	srand((unsigned) time(NULL));
}

/*
	Set the number of random points to be generated
*/
size_t setN(){
	size_t n = 0;
	puts("Set the number n > 0 of random points you want to generate:");

	while(n < 1){
		printf("Set n: ");
		scanf("%lu", &n);

		if(n <= 0)
			puts("It must be n > 0!");
	}

	return n;
}

/*
	Get random double from given range
*/
double getRandomDoubleFromRange(double min, double max){
	return ( (double)rand() * ( max - min ) ) / (double)RAND_MAX + min;
}

/*
	Generic Function keeping track of max value for rectangle evaluation
*/
double f(double x, double* y_max){
	//double y = x*x*x; //using f(x) = x^3 as sample function
	double y = fabs(sin(x));
	//double y = fabs(sin(exp(x)));
	if(y > *y_max){
		*y_max = y;
		//printf("New max: %lf\n", *y_max);
	}
	return y;
};
