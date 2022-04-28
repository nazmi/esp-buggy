// /**
//  * @author Aaron Berk
//  *
//  * @section LICENSE
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
//  * @section DESCRIPTION
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

#ifndef _PID_H_
#define _PID_H_

#include "debug.h"
#include "helper.h"
#include "mbed.h"

/**
 * @brief Proportional-integral-derivative controller
 */
class PID {
  public:
    /**
     * @brief Contructor
     * @details Sets default limits [0-3.3V], calculates tuning parameters, and sets manual mode with no bias.
     *
     * @param Kc - Proportional gain
     * @param tauI - Integral gain
     * @param tauD - Derivative gain
     * @param interval - PID calculation performed every interval seconds.
     */
    PID(float Kc, float tauI, float tauD, float interval);

    /**
     * @brief Scale from inputs to 0-100%
     *
     * @param inMin - The real world value corresponding to 0%
     * @param inMax - The real world value corresponding to 100%
     */
    void setInputLimits(float inMin, float inMax);

    /**
     * @brief Scale from outputs to 0-100%
     *
     * @param outMin - The real world value corresponding to 0%
     * @param outMax - The real world value corresponding to 100%
     */
    void setOutputLimits(float outMin, float outMax);

    /**
     * @brief Calculate PID constants.
     *
     * @details Allows parameters to be changed on the fly without ruining calculations.
     *
     * @param Kc - Proportional gain
     * @param tauI - Integral gain
     * @param tauD - Derivative gain
     */
    void setTunings(float Kc, float tauI, float tauD);

     /**
      * @brief Reset previous values of output and error.
      */
    void reset();

    /**
     * @brief Set the set point.
     *
     * @param sp The set point as a real world value.
     */
    void setSetPoint(float sp);

    /**
     * @brief Get the setpoint.
     * @return float Setpoint
     */
    float getSetPoint();

    /**
     * @brief Set the process value.
     *
     * @param pv The process value as a real world value.
     */
    void setProcessValue(float pv);

    /**
     * @brief Set the bias.
     *
     * @param bias The bias for the controller output.
     */
    void setBias(float bias);

    /**
     * @brief PID calculation.
     *
     * @return The controller output as a float between outMin and outMax.
     */
    float compute(void);

  protected:
    bool _usingFeedForward;

    // Actual tuning parameters used in PID calculation.
    float _Kc;
    float _tauR;
    float _tauD;

    // Raw tuning parameters.
    float _pParam;
    float _iParam;
    float _dParam;

    // The point we want to reach.
    float _setPoint;
    // The thing we measure.
    float _processVariable;
    float _prevProcessVariable;
    float _prevError;
    // The output that affects the process variable.
    float _controllerOutput;
    float _prevControllerOutput;

    // We work in % for calculations so these will scale from
    // real world values to 0-100% and back again.
    float _inMin;
    float _inMax;
    float _inSpan;
    float _outMin;
    float _outMax;
    float _outSpan;

    // The accumulated error, i.e. integral.
    float _accError;
    // The controller output bias.
    float _bias;

    // The exponential filter alpha
    float _a;

    // The interval between samples.
    float _tSample;

};

#endif  // _PID_H_
