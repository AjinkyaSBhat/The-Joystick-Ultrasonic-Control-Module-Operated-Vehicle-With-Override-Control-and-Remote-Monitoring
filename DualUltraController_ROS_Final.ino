#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

 // CHANNEL A-Pins 12,9,3 CHANNEL B- Pins 13,8,11 //
 
int trigPinA = 7;    
int echoPinA = 6;  
int trigPinB = 5 ;
int echoPinB = 2;    
long durationA, durationB, cmA, cmB;

ros::NodeHandle nh;

geometry_msgs::Twist motor_commands;
ros::Publisher ultracommand("Ultra_controller", &motor_commands);

 
                                              void setup() {
                                                
                                                                                      
                                                //Define inputs and outputs
                                                pinMode(trigPinA, OUTPUT);
                                                pinMode(echoPinA, INPUT);
                                                pinMode(trigPinB, OUTPUT);
                                                pinMode(echoPinB, INPUT);
                                                 //Setup Channel A
                                            
                                              pinMode(12, OUTPUT); //Initiates Motor Channel A pin
                                            
                                              pinMode(9, OUTPUT); //Initiates Brake Channel A pin
                                            
                                              //Setup Channel B
                                            
                                              pinMode(13, OUTPUT); //Initiates Motor Channel B pin
                                            
                                            pinMode(8, OUTPUT); //Initiates Brake Channel B pin
                                                                                                            
                                                                                                          
    nh.initNode();
  nh.advertise(ultracommand);  

  }
 
void loop()
{
  int speedA, speedB ;
  speedA=ultradistanceA();
  speedB=ultradistanceB();

  movemotorA(speedA);
  movemotorB(speedB);
 
  ultracommand.publish(&motor_commands);
  nh.spinOnce();
  while(motor_commands.angular.x==-1&&motor_commands.angular.y==-1)
  {
    speedA=ultradistanceA();
     speedB=ultradistanceB();

    movemotorA(speedA);
    movemotorB(speedB);
      //wait till change
  }

            
}

int ultradistanceA()
{
    digitalWrite(trigPinA, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinA, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinA, LOW);
 
  pinMode(echoPinA, INPUT);
  durationA = pulseIn(echoPinA, HIGH);
 
  // convert the time into a distance
  cmA = (durationA/2) / 29.1;
  
  delay(20);
  return(cmA);

   
}

int ultradistanceB()
{
    digitalWrite(trigPinB, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinB, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinB, LOW);
 
  pinMode(echoPinB, INPUT);
  durationB = pulseIn(echoPinB, HIGH);
 
  // convert the time into a distance
  cmB = (durationB/2) / 29.1;
  
return(cmB);
   
}

void movemotorA(int dist)
{
  int x; //movespeed proportional to distance of the hand from virtual trigger points
  if(dist>28)
  {
    digitalWrite(9,HIGH); //A brake ON
    motor_commands.angular.x=-1; //signifies out of range
  }
  else if(dist>10&&dist<18)
  {
    

    digitalWrite(9,HIGH); //A brake ON
    
    motor_commands.angular.x=dist;
    
    }
   else if(dist>18)
   {
    x=255-(28-dist)*10;
    digitalWrite(9,LOW); //A brake off
    digitalWrite(12,LOW);   //reverse
     
     analogWrite(3,x);//set speed 
     motor_commands.angular.x=dist;
    // delay(20);
     
   } 
   else if (dist <10)
   {
    x=255-dist*10;
     digitalWrite(9,LOW); //A brake off
      digitalWrite(12,HIGH);   //forward 
    
     analogWrite(3,x);//set speed 
     motor_commands.angular.x=dist;
     //delay(20);
     
   }
}


void movemotorB(int dist)
{
  int x;
  if(dist>28)
  {
    digitalWrite(8,HIGH); //A brake ON
    motor_commands.angular.y=-1;
  }
  else if(dist>10&&dist<18)
  {
    

    digitalWrite(8,HIGH); //A brake ON
    motor_commands.angular.y=dist;
    
    }
   else if(dist>18)
   {
    x=255-(28-dist)*10;
     digitalWrite(13,LOW); //rev B
 
  digitalWrite(8,LOW); //B brake off

 
  analogWrite(11,x); // set speed
   motor_commands.angular.y=dist; 
     //delay(20);
   } 
   else if (dist <10)
   {
    x=255-dist*10;
      digitalWrite(13,HIGH); //forward B
 
  digitalWrite(8,LOW); //B brake off

 
  analogWrite(11,x); // set speed 
    motor_commands.angular.y=dist;
//     delay(20);
   
   }
}
