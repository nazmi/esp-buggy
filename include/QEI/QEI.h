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

#ifndef QEI_H
#define QEI_H

#include "mbed.h"

/** @brief Mask for the previous state in determining direction */
#define PREV_MASK 0x1
/** @brief Mask for the current state in determining direction */
#define CURR_MASK 0x2
/** @brief XORing both states where both bits have changed */
#define INVALID 0x3

/**
 * @brief Handle reading pulses from encoder.
 */
class QEI {
  public:
    typedef enum Encoding {
        X2_ENCODING,
        X4_ENCODING
    } Encoding;

    /**
     * @brief Constructor.
     *
     * Reads the current values on channel A and channel B to determine the
     * initial state.
     *
     * Attaches the encode function to the rise/fall interrupt edges of
     * channels A and B to perform X4 encoding.
     *
     * Attaches the index function to the rise interrupt edge of channel index
     * (if it is used) to count revolutions.
     *
     * @param channelA mbed pin for channel A input.
     * @param channelB mbed pin for channel B input.
     * @param index    mbed pin for optional index channel input,
     *                 (pass NC if not needed).
     * @param pulsesPerRev Number of pulses in one revolution.
     * @param encoding The encoding to use. Uses X2 encoding by default. X2
     *                 encoding uses interrupts on the rising and falling edges
     *                 of only channel A where as X4 uses them on both
     *                 channels.
     */
    QEI(PinName channelA, PinName channelB, PinName index, int pulsesPerRev, Encoding encoding = X2_ENCODING);

    /**
     * @brief Reset the encoder.
     *
     * Sets the \link pulses_ pulses\endlink and \link revolutions_ revolutions\endlink count to zero.
     */
    void reset(void);

    /**
     * @brief Read the \link currState_ state\endlink of the encoder.
     *
     * @return The current state of the encoder as a 2-bit number, where:
     *         bit 1 = The reading from channel B
     *         bit 2 = The reading from channel A
     */
    int getCurrentState(void);

    /**
     * @brief Read the number of \link invalid_ invalid\endlink counts.
     *
     * @note Reading this resets the counter!
     *
     * @return the number of times the gray-code transition was violated.
     */
    int getInvalidCount(void);

    /**
     * @brief Read the number of \link pulses_ pulses\endlink recorded by the encoder.
     *
     * @return Number of pulses which have occured.
     */
    int getPulses(void);

    /**
     * @brief Read the number of \link revolutions_ revolutions\endlink recorded by the encoder on the index channel.
     *
     * @return Number of revolutions which have occured on the index channel.
     */
    int getRevolutions(void);

    /**
     * @brief Set the current \link pulses_ pulse count\endlink in case you want to init it.
     *
     * @param[in] newCount is the count to set it to.
     *
     */
    void setPulses(int newCount);

    /**
     * @brief Set the current \link revolutions_ revolution count\endlink in case you want to init it.
     *
     * @param[in] newRevs is the count to set it to.
     *
     */
    void setRevolutions(int newRevs);

  private:
    /**
     * @brief Update the pulse count.
     *
     * Called on every rising/falling edge of channels A/B.
     *
     * Reads the state of the channels and determines whether a pulse forward
     * or backward has occured, updating the count appropriately.
     */
    void encode(void);

    /**
     * @brief Called on every rising edge of channel index to update revolution
     * count by one.
     */
    void index(void);

    Encoding encoding_;

    InterruptIn channelA_;
    InterruptIn channelB_;
    InterruptIn index_;

    int pulsesPerRev_;
    int prevState_;
    int currState_;

    volatile int invalid_;

    volatile int pulses_;
    volatile int revolutions_;
};

#endif
