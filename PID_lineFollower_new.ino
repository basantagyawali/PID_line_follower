//BG Basanta Gyawali


//recent optimization is : baseSpeed : 160, Kp : 40, Kd : 10, Ki : 3

/*--Defining line array (IP sensors input pins)--*/

#define   S0      2         //Right most
#define   S1      3            
#define   S2      4         //Centre one 
#define   S3      5
#define   S4      6         //Left most


/*--Defination part for motor driver--*/

#define   ENA     9         //Enable pin A
#define   ENB     10        //Enable pin B
#define   INA1    11        //Input pin for right motor         
#define   INA2    12
#define   INB1    7        //Input pin for left motor
#define   INB2    8 


/* -- 1 - HIGH and 2 - LOW for forward MOTOR_ROTATION -- */


/*--Global variable declaration part--*/

const int baseSpeed = 160;
int prevError;
int P, I, D;


/*--Function declaration section--*/

int returnErr();
int PID(int error); 

void setup() {
  /*--Setup for motor pins--*/
  pinMode (INA1, OUTPUT);
  pinMode (INA2, OUTPUT);
  pinMode (INB1, OUTPUT);
  pinMode (INB2, OUTPUT);
  //For ENA and ENB defining pinMode is not necessary since they are for pwm and is default set to output

  /*--Setup for line array or IR Sensor--*/
  pinMode (S0, INPUT);
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  pinMode (S3, INPUT);
  pinMode (S4, INPUT);

  /*--Starting serial monitor--*/
  Serial.begin(9600);

  
}
void loop() {
  // put your main code here, to run repeatedly

  int error = returnErr();
  int Speed;

  if (error != 10) {
    Speed = PID(error);
    forward(Speed);
  } 
  
}

int PID(int error) {
  
  int kp = 40, kd = 10, ki = 3;
  int readSen[5];
  
  //To read data of the five sensors
  
  P = error;
  I = I + error;
  D = error - prevError;
  prevError = error;
  return (kp * P + ki * I + kd * D);
}

void forward(int a) {
  int A = constrain(baseSpeed + a, 0, 255); //total speed for motor A
  int B = constrain(baseSpeed - a, 0, 255); //total speed for motor B
  //Serial.print(A);Serial.print(",");Serial.println(B);
  analogWrite(ENA, A);//analog read data from 0 to 255 so we pass the value in it
  analogWrite(ENB, B);
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
}

 
/*--Function to return error--*/
int returnErr() {
  int readSen[5];
  
  //To read data of the five sensors
  for(int i = 0; i <= 4; i++) {
    readSen[i]=digitalRead(i+2);    //Here "i+2" is taken to take sensor data connected from S0 to S2
  }
  
  //error cases
  int state = (readSen[4]<<4 | readSen[3]<<3 | readSen[2]<<2 | readSen[1]<<1 | readSen[0]);
  const int errorLut[32] = { 10, 4, 2, 3, 0, 0, 1, 5, -2, 0, 0, 0, -1, 0, 0, 5, -4, 0, 0, 0, 0, 0, 0, 0, -3, 0, 0, 0, -5, 0, -5, 0};//here error 10 is a dummy error 
  return errorLut[state];       
}
