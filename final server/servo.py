# pip install RPi.GPIO
'''
toturial:https://juejin.cn/s/%E6%A0%91%E8%8E%93%E6%B4%BEpython%E6%8E%A7%E5%88%B6%E8%88%B5%E6%9C%BA

'''
import RPi.GPIO as GPIO
import time

# set the servo pin
GPIO_PIN = 12  # TODO: change to the servo pin
# set the PWM frequence to 50Hz
PWM_FREQ = 50


class Servo:
    def __init__(self, pin, freq=50):
        self.pin = pin
        self.freq = freq
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pin, GPIO.OUT)
        self.pwm = GPIO.PWM(self.pin, self.freq)
        self.pwm.start(0)

    def setAngle(self, angle):
        duty_cycle = (angle / 18.0) + 2.5
        self.pwm.ChangeDutyCycle(duty_cycle)

    def destroy(self):
        self.pwm.stop()
        GPIO.cleanup()


def test():
    servo = Servo(12)
    while True:
        # set the servo angle
        angle = float(input("input (0 - 180): "))
        servo.setAngle(angle)
