#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Int8
from sensor_msgs.msg import Joy

def send_Twist(data):
	move_cmd=Twist()
	
	if data.axes[0] !=0 :
		move_cmd.linear.x=data.axes[0]*255
		rospy.loginfo("X move %d",move_cmd.linear.x)
	if data.axes[1] !=0 :
		move_cmd.linear.y=data.axes[1]*255
		rospy.loginfo("Y move %d",move_cmd.linear.y)		
	pub.publish(move_cmd)
	

# Intializes everything
def start():
    
    global pub
 
    pub = rospy.Publisher('joy_cmdr', Twist, queue_size=10)
    
    # subscribed to joystick inputs on topic "joy"
    rospy.Subscriber("joy", Joy, send_Twist)
    # starts the node
    rospy.init_node('Joy2Arduino')
    rospy.spin()

if __name__ == '__main__':
    start()
