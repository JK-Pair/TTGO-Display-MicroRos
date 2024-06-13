// this code is example of publisher and subscriber
// https://github.com/micro-ROS/micro_ros_arduino/blob/galactic/examples/micro-ros_subscriber/micro-ros_subscriber.ino
// https://github.com/micro-ROS/micro_ros_arduino/blob/galactic/examples/micro-ros_publisher/micro-ros_publisher.ino
// run micro_ros_agent before connecting arduino
// docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:galactic serial --dev /dev/cu.usbmodem1422201 -v6

#include <Arduino.h>
#include <TFT_eSPI.h>

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int8.h>

rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;

// subscriber
rcl_subscription_t sub_state;
std_msgs__msg__Int8 state_msg;
rclc_executor_t executor_sub;

TFT_eSPI tft = TFT_eSPI();
#define MC_DATUM 4
String state_str = "S:";
String state_int = "0";

String force_str = "F:";
String force_int = "50 N";

String duty_str = "D:";
String duty_int = "50 %";

String mode_str = "Manual";

#define RCCHECK(fn)              \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
      error_loop();              \
    }                            \
  }
#define RCSOFTCHECK(fn)          \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
    }                            \
  }

/**
 * @brief loop to indicate error with blinking LED
 *
 */
void error_loop()
{
  while (1)
  {
    delay(100);
  }
}

/**
 * @brief subscription callback executed at receiving a message
 *
 * @param msgin
 */
void sub_state_callback(const void *msgin)
{
  const std_msgs__msg__Int8 *state_msg = (const std_msgs__msg__Int8 *)msgin;
  // (condition) ? (true exec):(false exec)
}

void setup()
{  
  tft.init();
  tft.setRotation(1);
  delay(2000);
  set_microros_transports();
  delay(2000);

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // // create node
  RCCHECK(rclc_node_init_default(&node, "display_node", "", &support));

  //create subscriber
  RCCHECK(rclc_subscription_init_default(
      &sub_state,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int8),
      "current_state"));

  RCCHECK(rclc_executor_init(&executor_sub, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor_sub, &sub_state, &state_msg, &sub_state_callback, ON_NEW_DATA));


  tft.fillScreen(TFT_RED);
  delay(50);
  tft.fillScreen(TFT_GREEN);
  delay(50);
  tft.fillScreen(TFT_BLUE);
  delay(50);
  tft.setTextSize(6);

}

void loop()
{
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor_sub, RCL_MS_TO_NS(100)));

  tft.drawString(state_str+String(state_msg.data), 5, 0);
  tft.setTextSize(3);
  tft.drawString(mode_str, 125, 15);
  tft.setTextSize(6);
  tft.drawString(force_str+force_int, 5, 48);
  tft.drawString(duty_str+duty_int, 5, 95);
}
