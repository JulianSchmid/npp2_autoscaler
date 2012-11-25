#ifndef PULSE_PATTERNSCALER_H
#define PULSE_PATTERNSCALER_H

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

#include <string>
#include <vector>
#include <pulse/Scaler.h>
#include <npp2.h>
#include <PatternSet.h>
#include <pulse/FileOpenException.h>
#include <pulse/ParseException.h>

namespace pulse {
	/** The PatternScaler allows the automatic scaling of input and target data of a NPP2::PatternSet.
	 */
	class PatternScaler {
	public:
#ifdef __APPLE__
#pragma mark Construction, desconstruction and copying
#endif
		/** \name Construction, desconstruction and copying
		 @{ */
		
		/** Default constructor - generates a PatternScaler without any scalers */
		PatternScaler();
		/** Copy constructor - clones all scalers 
		 *  \param other the PatternScaler that is beeing copied
		 */
		PatternScaler(const PatternScaler& other);
		/** Deconstructor - deletes all scalers */
		virtual ~PatternScaler();
		/** Copy operation - deletes all own scalers and clones all scalers of other 
		 *  \param other the PatternScaler that is beeing copied
		 *  \return reference to this 
		 */
		PatternScaler& operator=(const PatternScaler& other);
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Saving and loading functionality
#endif
		/** \name Saving and loading functionality
		 @{ */
		
		/** Loads scalers and scaler parameters for input and output from a file.
		 *  \param filename
		 */
		void loadFromFile(const std::string& filename) throw (FileOpenException, ParseException);
		/** Saves the scalers and scaler parameter into a file.
		 *  \param filename
		 */
		void saveToFile(const std::string& filename) const throw (FileOpenException);
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Adding of scalers
#endif
		/** \name Adding of scalers
		 @{ */
		
		/**
		 * Adds a scaler for the net input values
		 * \note the scaler is beeing cloned
		 */
		void addInputScaler(const Scaler& scaler);
		/**
		 * Adds a scaler for the net target values
		 * \note the scaler is beeing cloned
		 */
		void addTargetScaler(const Scaler& scaler);
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Updateing of scaler parameters
#endif
		/** \name Updateing of scaler parameters
		 @{ */
		
		/** Updates the input and target scaler with the data in the patternSet 
		 *  \pre patternSet.input_count == numInputDimensions()
		 *  \pre patternSet.target_count == numTargetDimensions()
		 *  \pre patternSet.pattern_count > 0
		 *  \param patternSet an PatternSet with not scaled values
		 */
		void updateScalers(const NPP2::PatternSet& patternSet);
		/** Updates the input scalers with the input data in the patternSet
		 *  \pre patternSet.input_count == numInputDimensions()
		 *  \pre patternSet.pattern_count > 0
		 *  \param patternSet an PatternSet with not scaled input values
		 */
		void updateInputScalers(const NPP2::PatternSet& patternSet);
		/** Updates the input scalers with the given data
		 *  \pre num > 0
		 *  \param in the pointer to the data, this is beeing accesed data[i][j] with \f$i \in {0...num-1}\f$ and \f$j \in {0...numInputDimensions()}\f$
		 *  \param num
		 */
		void updateInputScalers(double** in, size_t num);
		/** Updates the input scalers with the value in the vector.
		 *  \pre values.size() + start <= m_inputScalers.size()
		 *  \param values unscaled values
		 *  \param start the position in the vector where the values is that the first scaled should use to update itself
		 */
		void updateInputScalers(const std::vector<double>& values, size_t start);
		/** Updates the target scalers with the given data
		*  \pre patternSet.pattern_count > 0
		*  \param patternSet an PatternSet with not scaled target values
		*/
		void updateTargetScalers(const NPP2::PatternSet& patternSet);
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Reseting of scaler parameters
#endif
		/** \name Reseting of scaler parameters
		 @{ */
		/** Resets the input and target scalers with the given data
		 *  \pre patternSet.input_count == numInputDimensions()
		 *  \pre patternSet.target_count == numTargetDimensions()
		 *  \pre patternSet.pattern_count > 0
		 *  \param patternSet an PatternSet with not scaled values
		 */
		void resetScalers(const NPP2::PatternSet& patternSet);
		/** Resets the input scalers with the given data
		 *  \pre patternSet.target_count == numTargetDimensions()
		 *  \pre patternSet.pattern_count > 0
		 *  \param patternSet an PatternSet with not scaled target values
		 */
		void resetInputScalers(const NPP2::PatternSet& patternSet);
		
		/** Resets the target scalers with the given data
		 *  \pre patternSet.target_count == numTargetDimensions()
		 *  \pre patternSet.pattern_count > 0
		 *  \param patternSet an PatternSet with not scaled target values
		 */
		void resetTargetScalers(const NPP2::PatternSet& patternSet);
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Scaling of data
#endif
		/** \name Scaling of data
		 @{ */
		/** Scales the input and target values using the scalers
		 *  \note if you train a new net, you should call update or reset before scaling. This makes sure that no values are bigger/smaller then max/min.
		 *  \param patternSet an PatternSet with not scaled input and target values
		 */
		void scale(NPP2::PatternSet& patternSet) const;
		/** Scales the input values using the scalers
		 *  \note if you train a new net, you should call update or reset before scaling. This makes sure that no values are bigger/smaller then max/min.
		 *  \param patternSet an PatternSet with not scaled input values
		 */
		void scaleInputs(NPP2::PatternSet& patternSet) const;
		/** Scales the values with the input scalers.
		 *  \param values unscaled double values
		 */
		void scaleInput(double* values) const;
		/** Scales the values with the input scalers from start to numScalers.
		 *  \param values unscaled double values
		 *  \param start
		 *  \param numScalers
		 */
		void scaleInput(double* values, size_t start, size_t numScalers) const;
		/** Scales the values with the input scalers from start to numScalers and copies the results unto the heap.
		 *  \param input
		 *  \return pointer to the result data
		 */
		double* copyAndScaleInput(double const* input) const;
		/** Scales the target values using the scalers
		 *  \note if you train a new net, you should call update or reset before scaling. This makes sure that no values are bigger/smaller then max/min.
		 *  \param patternSet an PatternSet with not scaled target values
		 */
		void scaleTargets(NPP2::PatternSet& patternSet) const;

		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Restoring the original values from scaled values
#endif
		/** \name Restoring the original values from scaled values
		 @{ */
		
		/** Takes scaled values and return them to their original not scaled values
		 *  \param values scaled values
		 */
		void originalTargetValues(double* values) const;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Dimension informations
#endif
		/** \name Dimension informations
		 @{ */
		
		/** Returns the number of scalers for the input values of a NPP2::PatternSet */
		size_t numInputDimensions() const;
		/** Returns the number of scalers for the target values of a NPP2::PatternSet */
		size_t numTargetDimensions() const;
		
		/*@}*/
#ifdef __APPLE__
#pragma mark -
#endif
		
	private:
		std::vector<Scaler*> m_inputScalers;
		std::vector<Scaler*> m_targetScalers;
	};
}

#endif
