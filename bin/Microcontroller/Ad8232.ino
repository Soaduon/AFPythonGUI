//#include <movingAvg.h>
//#include <SimpleKalmanFilter.h>

float EMA_a_low = 0.3;    //initialization of EMA alpha
float EMA_a_high = 0.5;
 
int EMA_S_low = 0;        //initialization of EMA S
int EMA_S_high = 0;
 
int highpass = 0;
int bandpass = 0;

float deriv = 0;
float derivative;

float xn = 0;
float xn_1 = 0;
float xn_2 = 0;
float xn_3 = 0;
float xn_4 = 0;

float squaring;
unsigned long time;

float t_1 = 0;
float t_0 = 0;
float tinterval = 0;

//SimpleKalmanFilter simpleKalmanFilter(10, 10, 0.01);
//movingAvg avgTemp(10);

//
const PROGMEM unsigned int data_test[] = {312, 312, 309, 310, 274, 489, 617, 341, 321, 318, 318, 319, 323, 330, 340, 352, 366, 374, 375, 359, 330, 308, 300, 302, 303, 304, 308, 312, 313, 311, 313, 312, 312, 312, 313, 322, 329, 339, 327, 315, 309, 311, 312, 300, 310, 593, 488, 313, 318, 315, 318, 322, 329, 336, 345, 357, 369, 373, 370, 352, 327, 312, 303, 305, 306, 307, 311, 314, 318, 318, 320, 319, 317, 317, 325, 333, 343, 332, 321, 315, 315, 314, 302, 312, 606, 484, 322, 326, 323, 324, 328, 332, 339, 348, 361, 377, 383, 379, 357, 327, 311, 305, 305, 308, 310, 315, 319, 319, 318, 317, 316, 316, 321, 328, 341, 339, 327, 314, 311, 311, 311, 279, 489, 617, 343, 323, 321, 320, 322, 328, 335, 345, 355, 370, 380, 378, 363, 332, 311, 304, 304, 307, 308, 310, 311, 315, 316, 315, 316, 314, 322, 332, 340, 329, 313, 310, 308, 310, 298, 311, 606, 473, 313, 316, 317, 319, 325, 330, 338, 349, 360, 371, 376, 371, 350, 324, 311, 305, 304, 305, 310, 309, 313, 316, 317, 319, 326, 333, 341, 331, 316, 311, 309, 313, 289, 358, 633, 434, 308, 319, 315, 320, 326, 334, 342, 351, 363, 370, 374, 366, 342, 322, 311, 308, 307, 308, 311, 312, 315, 319, 321, 323, 328, 335, 339, 323, 312, 308, 308, 311, 271, 465, 635, 348, 314, 313, 314, 318, 322, 330, 338, 350, 361, 370, 371, 357, 332, 310, 304, 301, 301, 301, 307, 307, 310, 311, 313, 316, 321, 331, 328, 315, 305, 302, 304, 305, 268, 476, 631, 339, 313, 311, 310, 316, 322, 329, 336, 346, 363, 369, 371, 357, 329, 311, 302, 302, 301, 303, 305, 312, 313, 314, 316, 322, 325, 334, 327, 316, };
int j = 0;
int data_test_max = sizeof(data_test)/sizeof(int);
int data_test_counter = 0;


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  EMA_S_low = analogRead(A0);      //set EMA S for t=1
  EMA_S_high = analogRead(A0);
  
//  avgTemp.begin();  
  
}


/**
 * Simulating analog input signal.
 */
int sine_wave(double frequency) {
  double t = double(millis()) / 1000.0;
  return 1024.0 * sin(2.0 * 3.1415 * frequency * t);
}


/**
 * Main program loop.
 */
void loop() {

  int data;
  int sample = pgm_read_word_near(data_test+data_test_counter);
  data_test_counter++;
  
  if(data_test_counter >= data_test_max) data_test_counter = 0;
  
  
//  int sample = analogRead(A0);
  EMA_S_low = (EMA_a_low*sample) + ((1-EMA_a_low)*EMA_S_low);  //run the EMA
  EMA_S_high = (EMA_a_high*sample) + ((1-EMA_a_high)*EMA_S_high);
   
  bandpass = EMA_S_high - EMA_S_low;      //find the band-pass

  deriv = (2*bandpass*(sample) + (sample - 1) - (sample - 3) - (2*bandpass*(sample - 4)));
  derivative = deriv / 8;

  xn_4 = xn_3;
  xn_3 = xn_2;
  xn_2 = xn_1;
  xn_1 = xn;
  xn = bandpass;
  
  deriv = (2*abs(xn)) + abs(xn_1) - abs(xn_1) - abs(xn_3) - (2*(xn_4));
  derivative = deriv / 8;


  squaring = (abs(bandpass * derivative)* abs(bandpass * derivative))/100000;
  
//  Serial.println(squaring);
//
//  Serial.print("Time: ");
//  time = millis();
//  Serial.println(time);



  Serial.println (squaring);
  
//
//  time = millis();
//  Serial.print(" , ");
//  t_1 = t_0;
//  t_0 = time;
//
//  tinterval = time - t_1;
////
////  Serial.print(sample);
////  Serial.print(" ");
////  Serial.println(squaring);
//  Serial.println(tinterval);
//  
  
}
