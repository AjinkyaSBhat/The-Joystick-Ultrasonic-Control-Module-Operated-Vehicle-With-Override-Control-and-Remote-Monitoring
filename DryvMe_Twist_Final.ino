
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>


ros::NodeHandle  nh;
int vel_x , vel_y, distA, distB ;

void messageCb( const geometry_msgs::Twist& vel_cmd){
  
   vel_x=vel_cmd.linear.x;
   vel_y=vel_cmd.linear.y;
  if((vel_x!=0||vel_y!=0)&&(distA==-1&&distB==-1))
  {
  moveit(vel_x,vel_y);
  }
  else
  {
   STOP();
  }
}

void UltraCb( const geometry_msgs::Twist& ultra_cmd)
{
 distA=ultra_cmd.angular.x;
 distB=ultra_cmd.angular.y;
 
 ultramove(distA,distB);

 
}

ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
ros::Subscriber<geometry_msgs::Twist> sub2("Ultra_controller", &UltraCb );


void setup()
{ 
  Serial.begin(57600);
                                                                                                               //Setup Channel A
                                                                                                          
                                                                                                            pinMode(12, OUTPUT); //Initiates Motor Channel A pin
                                                                                                          
                                                                                                            pinMode(9, OUTPUT); //Initiates Brake Channel A pin
                                                                                                          
                                                                                                            //Setup Channel B
                                                                                                          
                                                                                                            pinMode(13, OUTPUT); //Initiates Motor Channel B pin
                                                                                                          
                                                                                                            pinMode(8, OUTPUT); //Initiates Brake Channel B pin
  
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(sub); // the topic published to by the joy2arduinotwist node
  nh.subscribe(sub2); // the topic published to by the Ultrasonic controller node
}

void loop()
{  
  nh.spinOnce();
  delay(1);
  //moveit(vel_x,vel_y);
}

//***********************************MOVEMENT FUNCTION-Ultasonic Control Module************************************////
void ultramove(int x, int y)
{
  if(x==-1&&y==-1)
  {
    //switch to joystick control if active
    STOP();
    ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
  
  }
  else
  {
    
      movemotorA(x);//pulls values from the calculating function
      movemotorB(y);
     delay(20);
      
      ros::Subscriber<geometry_msgs::Twist> sub2("Ultra_controller", &UltraCb );
    }

}

void movemotorA(int dist)
{
  int x; //movespeed proportional to distance of the hand from virtual trigger points
  if(dist>28||(dist>10&&dist<18))
  {
    
    analogWrite(3,0);
    
    }
   else if(dist>18)
   {
    x=255-(28-dist)*10;
    digitalWrite(9,LOW); //A brake off
    digitalWrite(12,LOW);   //reverse
     
     analogWrite(3,x);//set speed 
     
   } 
   else if (dist <10)
   {
    x=255-dist*10;
     digitalWrite(9,LOW); //A brake off
      digitalWrite(12,HIGH);   //forward 
    
     analogWrite(3,x);//set speed 
     
   }
}


void movemotorB(int dist)
{
  int x; //movespeed proportional to distance of the hand from virtual trigger points
  if(dist>28||(dist>10&&dist<18)) //purposeful deadband
  {
    analogWrite(11,0);
   
   
    }
   else if(dist>18)
   {
    x=255-(28-dist)*10;
    digitalWrite(8,LOW); //B brake off
     digitalWrite(13,LOW); //rev B
  analogWrite(11,x); // set speed 
     
   } 
   else if (dist <10)
   {
    x=255-dist*10;
    digitalWrite(8,LOW); //B brake off
      digitalWrite(13,HIGH); //forward B
      analogWrite(11,x); // set speed 
   
   }
}
/////////////////////////////////////////////////Ultrasonic Functions end///////////////////////////////////////////////////////////////////



//*****************************************************************MOVEMENT FUNCTION- JOYSTICK************************************************************************///
void moveit(int x, int y)
{
//Serial.println("Moving along X %d and Y %d",x,y);
//Serial.println("I am moving it, relax.");

//By default Joystick is mapped this way using the joy2arduino node. Applies fr the Logitech F710-may vary for others 
//                        ^ Y positive
//                        |
//                        |
// X positive <------------
// So X+ is left , X- is right. Aj 17/12. Check mapping for other JS, MAY CHANGE . If you don't know whats goin' on , this is likely it .

  if(x==0)
  {
      if(y>0)
      {
       
         //while(x==0&&y>0)
         //{
         mvFWD();
         
         ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
         
      }
      else if(y<0)
      {
        
         
          mvREV();
         ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
         //STOP();
      }
      else if (y==0)
      {
        STOP();
        //switch to the ultrasonic controller module, if active
       // ros::Subscriber<geometry_msgs::Twist> sub("Ultra_controller", &UltraCb );
      }// takes case of only forward or back movements
   }
   
   else if (y==0)
  {
      if(x>0)
      {
       
         trnLFT();
         
         ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
         
      }
      else if(x<0)
      {
        
         
          trnRGT();
         ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
         //STOP();
      }
      else if (x==0)
      {
        STOP();
       // ros::Subscriber<geometry_msgs::Twist> sub("Ultra_controller", &UltraCb );
      }// takes case of only right and left turns
   }
   
   else //differential motion
 
   {
     
     if(x>0&&y>0)
     {
       
       moveleftup(x,y);
       ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
     }
     else if(x>0&&y<0)
     {
       moveleftdown(x,-y);
       ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
     
     }
     else if(x<0&&y>0)
     {
       moverightup(-x,y);
       ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
     
     }
     else if(x<0&&y<0)
     {
       moverightdown(-x,-y);
       ros::Subscriber<geometry_msgs::Twist> sub("joy_cmdr", &messageCb );
     
     }
     else if(y==0&&x==0)
     {
       STOP();
       //ros::Subscriber<geometry_msgs::Twist> sub("Ultra_controller", &UltraCb );
     }

   
   
   }    
    
}
        
        
        
                                                                                                                  
                                                                                                             
                                                                                                             
                                                                                                             // Unidirection MOVE COMMANDS SET 
                                                                                                             
                                                                                                             
                                                                                                              void mvFWD (void)
                                                                                                             {
                                                                                                                                digitalWrite(12,HIGH);   //forward A
                                                                                                                                  digitalWrite(13,HIGH); //forward B
                                                                                                                                  digitalWrite(9,LOW); //A brake off
                                                                                                                                  digitalWrite(8,LOW); //B brake off
                                                                                                                                  
                                                                                                                                  analogWrite(11,255); //set speed
                                                                                                                                  analogWrite(3,255); // set speed
                                                                                                                                  delay(15);
                                                                                                                                  
                                                                                                                }
                                                                                                               void STOP (void)
                                                                                                               {
                                                                                                                 analogWrite(3,0);
                                                                                                                 analogWrite(11,0);
                                                                                                                  digitalWrite(9,HIGH); //A brake ON
                                                                                                                  digitalWrite(8,HIGH); // B Brake ON
                                                                                                                
                                                                                                                }
                                                                                                                void trnRGT ()
                                                                                                                {
                                                                                                                  
                                                                                                                        digitalWrite(12,LOW);   //forward A
                                                                                                                   digitalWrite(13,HIGH); //reverse B
                                                                                                                   digitalWrite(9,LOW); //A brake off
                                                                                                                   digitalWrite(8,LOW); //B brake off

                                                                                                                    analogWrite(11,255);//set speed MAX
                                                                                                                    analogWrite(3,255); // set speed MAX
                                                                                                                    delay(15); 
                                                                                                                   
                                                                                                                  }

                                                                                                                 void trnLFT ()
                                                                                                                 {
                                                                                                                        digitalWrite(12,HIGH);   //forward A
                                                                                                                   digitalWrite(13,LOW); //reverse B
                                                                                                                   digitalWrite(9,LOW); //A brake off
                                                                                                                   digitalWrite(8,LOW); //B brake off

                                                                                                                    analogWrite(11,255);//set speed MAX
                                                                                                                    analogWrite(3,255); // set speed MAX
                                                                                                                    delay(15);
                                                                                                                    
                                                                                                                  }

                                                                                                                 void mvREV()
                                                                                                                 {
                                                                                                                                  digitalWrite(12,LOW);   //reverse A
                                                                                                                                  digitalWrite(13,LOW); //reverse B
                                                                                                                                  digitalWrite(9,LOW); //A brake off
                                                                                                                                  digitalWrite(8,LOW);//B brake off
                                                                                                                                  
                                                                                                                                  analogWrite(11,127);//set speed
                                                                                                                                  analogWrite(3,127); // set speed
                                                                                                                                 delay(15);
                                                                                                                                  
                                                                                                                  }
                                                                                                                  
                                                                                                                  
                                                                                                                  
                                                                                                                  
                                                                                                                  
    // Differential movement command set
                                                                                                                  
 void moveleftup(int x, int y)
 {
 
  digitalWrite(12,HIGH);   //forward A
  digitalWrite(13,HIGH); //forward B
  digitalWrite(9,LOW); //A brake off
  digitalWrite(8,LOW); //B brake off

  analogWrite(11,x);//set speed MAX
  analogWrite(3,y); // set speed MAX
  delay(15);
 
 }
 
 void moveleftdown(int x, int y)
 {
 
  digitalWrite(12,LOW);   //forward A
  digitalWrite(13,LOW); //forward B
  digitalWrite(9,LOW); //A brake off
  digitalWrite(8,LOW); //B brake off

  analogWrite(11,x);//set speed MAX
  analogWrite(3,y); // set speed MAX
  delay(15);
 
 }
 void moverightup(int x, int y)
 {
 
  digitalWrite(12,HIGH);   //forward A
  digitalWrite(13,HIGH); //forward B
  digitalWrite(9,LOW); //A brake off
  digitalWrite(8,LOW); //B brake off

  analogWrite(11,x);//set speed MAX
  analogWrite(3,y); // set speed MAX
  delay(15);
 
 }
 void moverightdown(int x, int y)
 {
 
  digitalWrite(12,LOW);   //forward A
  digitalWrite(13,LOW); //forward B
  digitalWrite(9,LOW); //A brake off
  digitalWrite(8,LOW); //B brake off

  analogWrite(11,x);//set speed MAX
  analogWrite(3,y); // set speed MAX
  delay(15);
 
 }
