#pragma once

/*****************************************************************************
 
 Copyright (c) 2012, Julian Schmid,
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 - Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 - Neither the name of Julian Schmid nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 ****************************************************************************/

/*  Created by Julian Schmid on 25.11.2012.
 *  Last modified: 25.11.2012
 */

#include <vector>
#include <cstddef>
#include <string>

namespace pulse {
	/**
	 * A scaler scales double values to a target value range. 
	 * @note Before scaling any new data you should always call a update* or reset* methods. This makes sure that no new previosly unseen values result in a value that is outside the target value range.
	 */
	class Scaler {
	public:
#ifdef __APPLE__
#pragma mark Construction and deconstruction
#endif
		/** \name Construction and deconstruction
		 @{ */
		/**
		 * Deconstructor
		 */
		virtual ~Scaler() {}
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Updating of the scaler paramters
#endif
		/** \name Updating of the scaler paramters
		 @{ */
	
		/**
		 * Updates the scaling parameters of the scaler.
		 * The data is beeing accessed the following way: data[i*offset] with \f$i \in {0...num-1}\f$
		 * \note Calling this method DOES NOT reset previously determined baseline values based on previous update or reset operations.
		 * \pre num > 0
		 * \param data ptr to the not scaled data
		 * \param offset
		 * \param num number of entries
		 */
		virtual void updateScalingFactors(double const* data, size_t offset, size_t num) = 0;
		/**
		 * Updates the scaling parameters of the scaler.
		 * The data is beeing accessed the following way: data[i][offset] with \f$i \in {0...num-1}\f$
		 * \note Calling this method DOES NOT reset previously determined baseline values based on previous update or reset operations.
		 * \pre num > 0
		 * \param data ptr to the not scaled data
		 * \param offset
		 * \param num number of entries
		 */
		virtual void updateScalingFactors(double** const data, size_t offset, size_t num) = 0;
		/**
		 *  Updates the scaling parameters of the scaler based on a single value.
		 *  \note If you have more values that should be update, you should use one of the other functions for performance reasons.
		 *   Calling this method DOES NOT reset previously determined baseline values based on previous update or reset operations.
		 *  \pre num > 0
		 *  \param value the unscaled value
		 */
		virtual void updateScalingFactors(double value) = 0;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Reseting of the scaler paramters
#endif
		/** \name Reseting of the scaler paramters
		 @{ */
		
		/**
		 * Resets and updates the autoscaler data. This means old discovered min or max values will be discarded.
		 * The data is beeing accessed the following way: data[i*offset] with \f$i \in {0...num-1}\f$
		 * \note DOES reset previously determined baseline values based on previous update or reset operations.
		 * \param data ptr to the not scaled data
		 * \param offset offset between entries, will be used by data[offset*i]
		 * \param num number of entries
		 * \pre num > 0
		 */
		virtual void resetScalingFactors(double const* data, size_t offset, size_t num) = 0;
		/**
		 * Resets and updates the autoscaler data. This means old discovered min or max values will be discarded. 
		 * The data is beeing accessed the following way: data[i][offset] with \f$i \in {0...num-1}\f$
		 * \note DOES reset previously determined baseline values based on previous update or reset operations.
		 * \pre num > 0
		 * \param data
		 * \param offset
		 * \param num number of entries
		 */
		virtual void resetScalingFactors(double** const data, size_t offset, size_t num) = 0;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Scaling of data
#endif
		/** \name Scaling of data
		 @{ */
		
		/**
		 * Scales all the values in "in" and writes out the scalled values to "out". The data is beeing accessed the following way: in[inOffset*i] and out[outOffset*i] with \f$i \in {0...num-1}\f$
		 * \param in
		 * \param inOffset
		 * \param out
		 * \param outOffset
		 * \param num number of entries
		 */
		virtual void scale(double const* in, int inOffset, double* out, size_t outOffset, size_t num) const = 0;
		/**
		 * Scales the values in data based on the offset an the num. 
		 * The data is beeing accessed the following way: data[i][offset] with \f$i \in {0...num-1}\f$
		 * \param data pointer to the not scaled data that should be scaled
		 * \param offset
		 * \param num number of entries
		 */
		virtual void scale(double** data, size_t offset, size_t num) const = 0;
		/**
		 * Scales a single value
		 * \note You should use the other methods if more then one value should be scaled in one operation (for performance reasons)
		 * \param value the value that is beeing scaled
		 * \return the scaled value
		 */
		virtual double scale(double value) const = 0;

		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Callculating the original values from scaled values
#endif
		/** \name Callculating the original values from scaled values
		 @{ */

		/**
		 * Takes a scaled value and returns the original non scaled value.
		 * \param value the scaled value that should be returned to it's non scaled form.
		 * \return the original not scaled value
		 */
		virtual double originalValue(double value) const = 0;

		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Saving and loading functionality
#endif
		/** \name Saving and loading functionality
		 @{ */

		/**
		 * Returns the params of the scaler. This mainly serves as a way to save values learned by the reset and update functions.
		 * \param params the scaler params
		 */
		virtual void getParameters(std::vector<double>& params) const = 0;
		/**
		 * This method restores the values that have been previosly extracted by calling getParameters().
		 * \param params the parameters
		 */
		virtual void setParameters(const std::vector<double>& params) = 0;
		/**
		 * Returns a const pointer to the type name of a scaler.
		 * \return const pointer to the type name
		 */
		virtual const std::string& getTypeName() const = 0;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Cloning
#endif
		/** \name Cloning
		 @{ */
		
		/**
		 * Clones the scaler with all it's parameters
		 * \return a new scaler of the same type and with the same parameters.
		 */
		virtual Scaler* clone() const = 0;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#endif
	};
}