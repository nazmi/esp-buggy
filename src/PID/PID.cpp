// /**
//  * @author Aaron Berk
//  *
//  * @section LICENSE1
//  *
//  * Copyright (c) 2010 ARM Limited
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy
//  * of this software and associated documentation files (the "Software"), to deal
//  * in the Software without restriction, including without limitation the rights
//  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  * copies of the Software, and to permit persons to whom the Software is
//  * furnished to do so, subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in
//  * all copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  * THE SOFTWARE.
//  *
//  * @section DESCRIPTION1
//  *
//  * A PID controller is a widely used feedback controller commonly found in
//  * industry.
//  *
//  * This library is a port of Brett Beauregard's Arduino PID library:
//  *
//  *  http://www.arduino.cc/playground/Code/PIDLibrary
//  *
//  * The wikipedia article on PID controllers is a good place to start on
//  * understanding how they work:
//  *
//  *  http://en.wikipedia.org/wiki/PID_controller
//  *
//  * For a clear and elegant explanation of how to implement and tune a
//  * controller, the controlguru website by Douglas J. Cooper (who also happened
//  * to be Brett's controls professor) is an excellent reference:
//  *
//  *  http://www.controlguru.com/
//  */

#include "PID.h"

PID::PID(float Kc, float tauI, float tauD, float interval) {

    _usingFeedForward = false;

    // Default the limits to the full range of I/O: 3.3V
    // Make sure to set these to more appropriate limits for your application
    setInputLimits(0.0f, 3.3f);
    setOutputLimits(0.0f, 3.3f);

    _tSample = interval;
    setTunings(Kc, tauI, tauD);

    _setPoint = 0.0f;
    _processVariable = 0.0f;
    _controllerOutput = 0.0f;
    _prevControllerOutput = 0.0f;
    _prevError = 0.0f;

    _a = 0.7f;
    _accError = 0.0f;
    _bias = 0.0f;
}

void PID::setInputLimits(float inMin, float inMax) {

    // Make sure we haven't been given impossible values.
    if (inMin >= inMax)
        ERROR("Invalid input limits");

    // Rescale the working variables to reflect the changes.
    _accError *= (inMax - inMin) / _inSpan;

    _inMin = inMin;
    _inMax = inMax;
    _inSpan = inMax - inMin;
}

void PID::setOutputLimits(float outMin, float outMax) {

    // Make sure we haven't been given impossible values.
    if (outMin >= outMax)
        ERROR("Invalid output limits");

    // Rescale the working variables to reflect the changes.
    _prevControllerOutput *= (outMax - outMin) / _outSpan;

    // Make sure the working variables are within the new limits.
    _prevControllerOutput = clamp(_prevControllerOutput, -1.0f, 1.0f);

    _outMin = outMin;
    _outMax = outMax;
    _outSpan = outMax - outMin;
}

void PID::setTunings(float Kc, float tauI, float tauD) {

    // Verify that the tunings make sense.
    if (Kc == 0.0f || tauI < 0.0f || tauD < 0.0f) {
        ERROR("Invalid tuning parameters");
    }

    // Store raw values to hand back to user on request.
    _pParam = Kc;
    _iParam = tauI;
    _dParam = tauD;

    float tempTauR;

    if (tauI == 0.0f) {
        tempTauR = 0.0f;
    } else {
        tempTauR = (1.0f / tauI) * _tSample;
    }

    _Kc = Kc;
    _tauR = tempTauR;
    _tauD = tauD / _tSample;
}

void PID::reset() {

    _prevError = 0.0f;
    _prevControllerOutput = 0.0f;
    _accError = 0.0f;
}

void PID::setSetPoint(float sp) {
    _setPoint = sp;
}

float PID::getSetPoint() {
    return _setPoint;
}

void PID::setProcessValue(float pv) {
    _processVariable = pv;
}

void PID::setBias(float bias) {
    _bias = bias;
    _usingFeedForward = 1;
}

float PID::compute() {

    // Pull in the input and setpoint, and scale them into percent span.
    float scaledPV = clamp((_processVariable - _inMin) / _inSpan, 0.0f, 1.0f);
    float scaledSP = clamp((_setPoint - _inMin) / _inSpan, 0.0f, 1.0f);
    float error = scaledSP - scaledPV;
    float dError = 0.0;
    float scaledBias = 0.0;

    // Exponential Filter
    if (_tauD > 0) {
        dError = _a * error + (1 - _a) * _prevError;
    }

    // Check and see if the output is pegged at a limit and only
    // integrate if it is not. This is to prevent reset-windup.
    if (!(_prevControllerOutput >= 1 && error > 0) && !(_prevControllerOutput <= 0 && error < 0)) {
        _accError += error;
    }

    if (_usingFeedForward) {
        scaledBias = (_bias - _outMin) / _outSpan;
    }

    // Perform the PID calculation.
     _controllerOutput = scaledBias + _Kc * (error + (_tauR * _accError) + _tauD * dError / _tSample);

    DEBUGLOG("[ %.5f %.5f %.5f %.5f %.5f ", scaledPV, scaledSP, error, dError, _controllerOutput);
    // Make sure the computed output is within output constraints.
    _controllerOutput = clamp(_controllerOutput, -1.0f, 1.0f);

    // Remember this output for the windup check next time.
    _prevControllerOutput = _controllerOutput;
    // Remember the input for the derivative calculation next time.
    _prevError = dError;

    DEBUGLOG("%.5f] \n", (_controllerOutput * _outSpan) + _outMin);
    // Scale the output from percent span back out to a real world number.
    return ((_controllerOutput * _outSpan) + _outMin);
}
