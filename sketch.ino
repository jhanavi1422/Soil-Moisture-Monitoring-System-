print(" This is the program for Soil Moisture Monitoring System")

# STEP 1 : IMPORT MODULES OR LIBRARY
from machine import Pin, I2C, ADC, PWM
import ssd1306
import time

#STEP 2: DECLARE CONNECTION
# ESP32 Pin assignment 

buzzer = PWM(Pin(26), Pin.OUT)
soil_sensor = ADC(Pin(34))  # Analog pin for soil moisture sensor
orangeled = Pin(32,Pin.OUT)
yellowled = Pin(33,Pin.OUT)
greenled = Pin(25,Pin.OUT)

#STEP 2.2 : DECLARE THE CONNECTION OLED
i2c = I2C(0, scl=Pin(22), sda=Pin(21))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)


#STEP 3 : THE PROCESS
while True:
   # Read the soil moisture sensor value
    sensorValue = soil_sensor.read()
    
    # Map the sensor value to a percentage (assuming 2165-3135 range)
    humidityPercent = int((sensorValue - 2165) / (3135 - 2165) * 100)
    humidityPercent = max(0, min(humidityPercent, 100))  # Clamp to range 0-100

    # Determine the state based on humidity percentage
    if humidityPercent < 40:
        print("WET")
        orangeled.on()                              # Turn on red LED
        yellowled.off()                             # Turn off yellow LED
        greenled.off()                              # Turn off green LED
        buzzer.init( freq = 400, duty = 50)         # Turn on buzzer
        time.sleep(0.5)                             # Keep buzzer on for half a second
        buzzer.init(freq=1, duty=0)                 # Turn off buzzer

    elif humidityPercent > 60:
        print("DRY")
        orangeled.off()                            # Turn off red LED
        yellowled.off()                            # Turn off yellow LED
        greenled.on()                              # Turn on green LED
        buzzer.init( freq = 2000, duty = 50)        # Turn on buzzer at higher frequency
        time.sleep(0.5)                            # Keep buzzer on for half a second
        buzzer.init(freq=1, duty=0)                # Turn off buzzer

    else:
        print("IDEAL")
        orangeled.off()                   # Turn off red LED
        yellowled.on()                    # Turn on yellow LED
        greenled.off()                    # Turn off green LED
        buzzer.init(freq=1, duty=0)       # Ensure buzzer is off

    # Display the status on the OLED display
    oled.fill(0)  # Clear the display
    oled.text("Soil Moisture", 0, 0)
    oled.text("Percentage: {}%".format(humidityPercent), 0, 20)
    oled.show()

    time.sleep(1)  # Delay before next reading cycle
