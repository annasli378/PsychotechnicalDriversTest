# Psychotechnical Driver's Test

<!-- PROJECT LOGO -->
<br />
  <p align="left">
    The system allows testing reaction time to a specific light stimulus and measuring and presenting the results to the user. In addition, the number of mistakes made by the test subject and the designated minimum reaction time are counted.
</p>


<!-- ABOUT THE PROJECT -->
## About The Project

![Layout](https://github.com/annasli378/PsychotechnicalDriversTest/main/img/layout.PNG)



![Schematic generated using SimulIDE software.](https://github.com/annasli378/
)


### Components used
* Arduino Uno
* 1602A 16x2 LCD display
* board
* LEDs: red, yellow and green
* 220, 330 Ohm, 10k Ohm resistors
* rotary potentiometer
* three buttons

### Build with
* Arduino IDE
* Microchip Studio
* C language

### Features
* There are three possible tests to choose from, differing in the order in which the LEDs light up. This order is fixed permanently in memory, while the test itself is selected on the basis of a generated random number (0, 1 or 2),
* For each position in the sequence, a particular diode will be lit, the program will wait 3 seconds and then extinguish it. If the yellow diode is lit, the user should refrain from pressing any button - this will increase the error counter,
* If the red or green LED is to be lit - the user is expected to press the corresponding button. Lack of response or pressing another button will increase the error counter and result in entering into the table times the maximum result, i.e. 3000 ms. If the user presses the button correctly, the stimulus response time is calculated and entered into the scoreboardresults,
* Finally, an array of the resulting times is obtained, the minimum determined from it (the shortest reaction time to light the button) in and the number of errors made are presented to the to the user at the end of the program operation

### How it works

After starting, the program waits for a signal from the user - pressing any button. Then the user has about 3 seconds to get ready, after which the time the display is cleared of text and the actual test begins, during which the which the LEDs light up in a specific order:

![Built layout](https://github.com/annasli378/
)

![Built layout](https://github.com/annasli378/
)

The proper reaction of the test subject to a green or red LED is to press the corresponding button as soon as possible. In the case of yellow, the user should refrain from any reaction. The reaction time is measured as the period of time between when the LED lights up and when the button is pressed.

When the entire sequence is complete, the LEDs turn off and the LCD display shows the results, i.e. the minimum reaction time recorded during the test and the number of errors made by the user during the test:

![Built layout](https://github.com/annasli378/
)








<!-- README created using the following template -->
<!-- https://github.com/othneildrew/Best-README-Template -->
