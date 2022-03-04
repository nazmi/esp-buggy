/**
 * @author Aaron Berk
 *
 * @section LICENSE
 *
 * Derivative work created in Jan 2021 by D.Smart, which 
 * has the following changes:
 *  + Update for MBED OS 6 ('callback' added to member ISRs)
 *  + Added counter for non-grey-code transitions
 *  + Added set functions to initialize counts to known values
 *  + Added get functions for error counters
 *
 * No copyright claim is being made on these changes.
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * QEI.
 *
 * A quadrature encoder consists of two code tracks on a disc which are 90
 * degrees out of phase. It can be used to determine how far a wheel has
 * rotated, relative to a known starting position.
 *
 * Only one code track changes at a time leading to a more robust system than
 * a single track, because any jitter around any edge won't cause a state
 * change as the other track will remain constant.
 *
 * Encoders can be a homebrew affair, consisting of infrared emitters/receivers
 * and paper code tracks consisting of alternating black and white sections;
 * alternatively, complete disk and PCB emitter/receiver encoder systems can
 * be bought, but the interface, regardless of implementation is the same.
 * Rotational position in degrees can be calculated by:
 *
 * \f$ rotational\ position\ =\  \frac{pulse\ count}{X\ \cdot \ N} \cdot \ 360\\\f$
 *
 * Where X is the encoding type [e.g. X4 encoding => X=4], and N is the number
 * of pulses per revolution.
 *
 * Linear position can be calculated by:
 *
 * \f$ linear\ position\ =\  \frac{pulse\ count}{X\ \cdot \ N} \cdot \ \frac{1}{PPI}\\\f$
 *
 * Where X is encoding type [e.g. X4 encoding => X=44], N is the number of
 * pulses per revolution, and PPI is pulses per inch, or the equivalent for
 * any other unit of displacement. PPI can be calculated by taking the
 * circumference of the wheel or encoder disk and dividing it by the number
 * of pulses per revolution.
 */

#include "QEI.h"

QEI::QEI(PinName channelA,
         PinName channelB,
         PinName index,
         int pulsesPerRev,
         Encoding encoding) : channelA_(channelA), channelB_(channelB),
        index_(index) {

    pulses_       = 0;
    revolutions_  = 0;
    invalid_      = 0;
    pulsesPerRev_ = pulsesPerRev;
    encoding_     = encoding;

    //Workout what the current state is.
    int chanA = channelA_.read();
    int chanB = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);
    prevState_ = currState_;

    //X2 encoding uses interrupts on only channel A.
    //X4 encoding uses interrupts on      channel A,
    //and on channel B.
    channelA_.rise(callback(this, &QEI::encode));
    channelA_.fall(callback(this, &QEI::encode));

    //If we're using X4 encoding, then attach interrupts to channel B too.
    if (encoding == X4_ENCODING) {
        channelB_.rise(callback(this, &QEI::encode));
        channelB_.fall(callback(this, &QEI::encode));
    }
    //Index is optional.
    if (index !=  NC) {
        index_.rise(callback(this, &QEI::index));
    }

}

void QEI::setPulses(int newCount) {
    pulses_     = newCount;
}

void QEI::setRevolutions(int newRevs) {
    revolutions_    = newRevs;
}

void QEI::reset(void) {

    pulses_      = 0;
    revolutions_ = 0;
    invalid_     = 0;

}

int QEI::getInvalidCount(void) {
    int r = invalid_;
    invalid_ = 0;
    return r;
}

int QEI::getCurrentState(void) {

    return currState_;

}

int QEI::getPulses(void) {

    return pulses_;

}

int QEI::getRevolutions(void) {

    return revolutions_;

}

// +-------------+
// | X2 Encoding |
// +-------------+
//
// When observing states two patterns will appear:
//
// Counter clockwise rotation:
//
// 10 -> 01 -> 10 -> 01 -> ...
//
// Clockwise rotation:
//
// 11 -> 00 -> 11 -> 00 -> ...
//
// We consider counter clockwise rotation to be "forward" and
// counter clockwise to be "backward". Therefore pulse count will increase
// during counter clockwise rotation and decrease during clockwise rotation.
//
// +-------------+
// | X4 Encoding |
// +-------------+
//
// There are four possible states for a quadrature encoder which correspond to
// 2-bit gray code.
//
// A state change is only valid if of only one bit has changed.
// A state change is invalid if both bits have changed.
//
// Clockwise Rotation ->
//
//    00 01 11 10 00
//
// <- Counter Clockwise Rotation
//
// If we observe any valid state changes going from left to right, we have
// moved one pulse clockwise [we will consider this "backward" or "negative"].
//
// If we observe any valid state changes going from right to left we have
// moved one pulse counter clockwise [we will consider this "forward" or
// "positive"].
//
// We might enter an invalid state for a number of reasons which are hard to
// predict - if this is the case, it is generally safe to ignore it, update
// the state and carry on, with the error correcting itself shortly after.
void QEI::encode(void) {

    int change = 0;
    int chanA  = channelA_.read();
    int chanB  = channelB_.read();

    //2-bit state.
    currState_ = (chanA << 1) | (chanB);

    if (encoding_ == X2_ENCODING) {

        //11->00->11->00 is counter clockwise rotation or "forward".
        if ((prevState_ == 0x3 && currState_ == 0x0) ||
                (prevState_ == 0x0 && currState_ == 0x3)) {

            pulses_++;

        }
        //10->01->10->01 is clockwise rotation or "backward".
        else if ((prevState_ == 0x2 && currState_ == 0x1) ||
                 (prevState_ == 0x1 && currState_ == 0x2)) {

            pulses_--;

        }
        else {
            invalid_++;
        }

    } else if (encoding_ == X4_ENCODING) {

        //Entered a new valid state.
        if (((currState_ ^ prevState_) != INVALID) && (currState_ != prevState_)) {
            //2 bit state. Right hand bit of prev XOR left hand bit of current
            //gives 0 if clockwise rotation and 1 if counter clockwise rotation.
            change = (prevState_ & PREV_MASK) ^ ((currState_ & CURR_MASK) >> 1);

            if (change == 0) {
                change = -1;
            }

            pulses_ -= change;
        }

    }

    prevState_ = currState_;

}

void QEI::index(void) {

    revolutions_++;

}
