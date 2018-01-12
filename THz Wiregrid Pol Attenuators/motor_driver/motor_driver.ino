#include <Stepper.h>
#include <EEPROM.h>

const int mStepsPerRev = 200; //mStepsPerRev of motor not entire window
const int wStepsPerRev = 4268; //steps per revolution of polarizing window

// initialize the stepper library on the motor shield
Stepper myStepper(mStepsPerRev, 12, 13);

// give the motor control pins names:
const int pwmA = 3;
const int pwmB = 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;

//all variables used in the loop function
int steps = 0; //number of steps to move
int previousStep = 0; //works with currentStep to check if motor moved indicated steps
int currentStep = 0;
bool isMoving = false; //should we keep turning the motor?
bool previousDir = true; //previous and current Dir handle correction of changing directions
bool currentDir = true; // true is clockwise false is counterclockwise
int stepAddress = 0; //EEPROM address int for logging currentStep
int rotAddress = sizeof(int); //EEPROM address int for logging isClockwise. Needs to be offset by the size of the previous data
//int correctionStep = 23; //correction step counter for switching rotation directions
//int currentCorrectionStep = 0;
String serialBuffer;

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(5);

    //gets the current step from previous use
//    float f = 0.00f;
//    EEPROM.get(stepAddress, f);
//    currentStep = (int)f;
    EEPROM.get(stepAddress, currentStep);
    previousStep = currentStep;

    //gets currentDir from previous use
    bool b;
    EEPROM.get(rotAddress, b);
    previousDir = b;
    currentDir = b;

    // set the PWM and brake pins so that the direction pins  // can be used to control the motor:
    pinMode(pwmA, OUTPUT);
    pinMode(pwmB, OUTPUT);
    pinMode(brakeA, OUTPUT);
    pinMode(brakeB, OUTPUT);
    digitalWrite(pwmA, HIGH);
    digitalWrite(pwmB, HIGH);
    digitalWrite(brakeA, LOW);
    digitalWrite(brakeB, LOW);
}

void loop()
{

    if (Serial.available() > 0) {
        serialBuffer = Serial.readString();

        if (serialBuffer.substring(0, 1) == "m") {
            //input "m***" or "m-***"and it will move *** steps or -***steps

            if (serialBuffer.substring(1, 2) == "-") {
                //counter clockwise rotatiation
                steps = (-1) * (serialBuffer.substring(2).toInt());
                currentDir = false;
            }

            else {
                //clockwise rotation
                steps = serialBuffer.substring(1).toInt();
                currentDir = true;
            }

            isMoving = true;
        }

        else if (serialBuffer.substring(0, 2) == "sm") {
            //stops motor whether or not it finished its loop
            isMoving = false;
        }

        else if (serialBuffer.substring(0, 2) == "gs") {
            //gets the current step that the motor is at
            Serial.println(currentStep);
        }

        else if (serialBuffer.substring(0, 1) == "s") {
            //sets the motor to a specified step: "s***" sets it to step ***
            //generally used to reset the window back to zero degrees
            steps = serialBuffer.substring(1).toInt() - currentStep;
            isMoving = true;
            currentDir = steps > 0;
        }

        else if (serialBuffer.substring(0, 2) == "ib") {
            //indicates whether the motor is still moving or not
            Serial.println(isMoving);
        }

        else if (serialBuffer.substring(0, 1) == "e") {
            //either sets the EEPROM to currentStep and currentDir: "e"
            //or sets EEPROM to zero: "ec"

//            float f = (float)currentStep;
//            bool b = currentDir;
            if (serialBuffer.substring(1, 2) == "c") {
                EEPROM.put(stepAddress, 0);
                currentStep = 0;
                previousStep = 0;
            }
            else {
                EEPROM.put(stepAddress, currentStep);
                EEPROM.put(rotAddress, currentDir);
            }
        }

//        Serial.flush();
    }

    if (isMoving) {
        if (steps != 0) {
            int toMove = int(currentDir == false) * 2 - 1;
            myStepper.step(-1 * toMove);
            steps += toMove;
            currentStep -= toMove;
            // Always store the position after moving
            EEPROM.put(stepAddress, currentStep);
        }
        else {
            isMoving = false;
            // save the direction after finishing a move. Should probably be
            // done at the start, though.
            EEPROM.put(rotAddress, currentDir);
        }

        //  if (currentDir != previousDir){ possible backlash correction
        //    if(currentCorrectionStep != correctionStep){
        //      myStepper.step(-1*toMove);
        //      currentCorrectionStep += 1;
        //      Serial.println(currentCorrectionStep);
        //    }
        //    else{
        //      previousDir = currentDir;
        //      currentCorrectionStep = 0;
        //    }
        //  }
    }

    delay(15);
}

