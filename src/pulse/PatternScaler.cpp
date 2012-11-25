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

#include <pulse/PatternScaler.h>

#include <cassert>
#include <sstream>
#include <pulse/ScalerFactory.h>
#include <pulse/ScalerSaver.h>

namespace pulse {
#ifdef __APPLE__
#pragma mark Construction, desconstruction and copying
#endif
	/** \name Construction, desconstruction and copying
	 @{ */
	PatternScaler::PatternScaler() {
	
	}
	PatternScaler::PatternScaler(const PatternScaler& other) {
		{
			std::vector<Scaler*>::const_iterator it;
			for(it = other.m_inputScalers.begin(); it != other.m_inputScalers.end(); it++) {
				m_inputScalers.push_back((*it)->clone());
			}
		}
		{
			std::vector<Scaler*>::const_iterator it;
			for(it = other.m_targetScalers.begin(); it != other.m_targetScalers.end(); it++) {
				m_targetScalers.push_back((*it)->clone());
			}
		}
	}
	PatternScaler::~PatternScaler() {
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				delete (*it);
			}
		}
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				delete (*it);
			}
		}
	}
	PatternScaler& PatternScaler::operator=(const PatternScaler& other) {
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				delete (*it);
			}
		}
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				delete (*it);
			}
		}
		m_inputScalers.clear();
		m_targetScalers.clear();
		{
			std::vector<Scaler*>::const_iterator it;
			for(it = other.m_inputScalers.begin(); it != other.m_inputScalers.end(); it++) {
				m_inputScalers.push_back((*it)->clone());
			}
		}
		{
			std::vector<Scaler*>::const_iterator it;
			for(it = other.m_targetScalers.begin(); it != other.m_targetScalers.end(); it++) {
				m_targetScalers.push_back((*it)->clone());
			}
		}
		return *this;
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Saving and loading functionality
#endif
	/** \name Saving and loading functionality
	 @{ */
	void PatternScaler::loadFromFile(const std::string& filename) throw (FileOpenException, ParseException) {
		//delete the old scalers
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				delete (*it);
			}
		}
		{
			std::vector<Scaler*>::iterator it;
			for(it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				delete (*it);
			}
		}
		m_inputScalers.clear();
		m_targetScalers.clear();
	
		//load
		ScalerFactory factory(filename);
		size_t numInputs = factory.getMaxId("input");
		size_t numTargets = factory.getMaxId("target");
		assert(numInputs > 0);
		assert(numTargets > 0);
		for (size_t i = 1; i <= numInputs; i++) {
			std::stringstream sstr;
			sstr<<"input"<<i;
			Scaler* s = factory.getScaler(sstr.str());
			addInputScaler(*s);
			delete s;
		}
		for (size_t i = 1; i <= numTargets; i++) {
			std::stringstream sstr;
			sstr<<"target"<<i;
			Scaler* s = factory.getScaler(sstr.str());
			addTargetScaler(*s);
			delete s;
		}
	}
	void PatternScaler::saveToFile(const std::string& filename) const throw (FileOpenException) {
	
		ScalerSaver saver(filename);
		{
			std::vector<Scaler*>::const_iterator it;
			size_t i = 1;
			for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				std::stringstream sstr;
				sstr<<"input"<<i;
				saver.saveScaler(sstr.str(), *it);
				i++;
			}
		}
		{
			std::vector<Scaler*>::const_iterator it;
			size_t i = 1;
			for (it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				std::stringstream sstr;
				sstr<<"target"<<i;
				saver.saveScaler(sstr.str(), *it);
				i++;
			}
		}
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Adding of scalers
#endif
	/** \name Adding of scalers
	 @{ */
	void PatternScaler::addInputScaler(const Scaler& scaler) {
		m_inputScalers.push_back(scaler.clone());
	}
	void PatternScaler::addTargetScaler(const Scaler& scaler) {
		m_targetScalers.push_back(scaler.clone());
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Updateing of scaler parameters
#endif
	/** \name Updateing of scaler parameters
	 @{ */
	void PatternScaler::updateScalers(const NPP2::PatternSet& patternSet) {
		updateInputScalers(patternSet);
		updateTargetScalers(patternSet);
		
	}
	void PatternScaler::updateInputScalers(const NPP2::PatternSet& patternSet) {
		assert(patternSet.input_count == m_inputScalers.size());
		assert(patternSet.pattern_count > 0);
		{
			size_t i = 0;
			std::vector<Scaler*>::iterator it;
			for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				(*it)->updateScalingFactors(patternSet.input, i, patternSet.pattern_count);
				i++;
			}
		}
		
	}
	void PatternScaler::updateInputScalers(double** in, size_t num) {
		assert(num > 0);
		{
			size_t i = 0;
			std::vector<Scaler*>::iterator it;
			for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				(*it)->updateScalingFactors(in, i, num);
				i++;
			}
		}
	}
	void PatternScaler::updateInputScalers(const std::vector<double>& values, size_t start) {
		assert(values.size() + start <= m_inputScalers.size());
		
		std::vector<Scaler*>::iterator it;
		for (size_t i = 0; i < values.size(); i++) {
			m_inputScalers[i+start]->updateScalingFactors(values[i]);
		}
	}
	void PatternScaler::updateTargetScalers(const NPP2::PatternSet& patternSet) {
		assert(patternSet.target_count == m_targetScalers.size());
		{
			size_t i = 0;
			std::vector<Scaler*>::iterator it;
			for (it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				(*it)->updateScalingFactors(patternSet.target, i, patternSet.pattern_count);
				i++;
			}
		}
		
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Reseting of scaler parameters
#endif
	/** \name Reseting of scaler parameters
	 @{ */
	void PatternScaler::resetScalers(const NPP2::PatternSet& patternSet) {
		resetInputScalers(patternSet);
		resetTargetScalers(patternSet);
	}
	void PatternScaler::resetInputScalers(const NPP2::PatternSet& patternSet) {
		assert(patternSet.input_count == m_inputScalers.size());
		assert(patternSet.pattern_count > 0);
		{
			size_t i = 0;
			std::vector<Scaler*>::iterator it;
			for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				(*it)->resetScalingFactors(patternSet.input, i, patternSet.pattern_count);
				i++;
			}
		}
	}
	void PatternScaler::resetTargetScalers(const NPP2::PatternSet& patternSet) {
		assert(patternSet.target_count == m_targetScalers.size());
		{
			size_t i = 0;
			std::vector<Scaler*>::iterator it;
			for (it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				(*it)->resetScalingFactors(patternSet.target, i, patternSet.pattern_count);
				i++;
			}
		}
		
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Scaling of data
#endif
	/** \name Scaling of data
	 @{ */
	void PatternScaler::scale(NPP2::PatternSet& patternSet) const {
		scaleInputs(patternSet);
		scaleTargets(patternSet);
	}
	void PatternScaler::scaleInputs(NPP2::PatternSet& patternSet) const {
		
		assert(patternSet.input_count == m_inputScalers.size());
		{
			size_t i = 0;
			std::vector<Scaler*>::const_iterator it;
			for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
				(*it)->scale(patternSet.input, i, patternSet.pattern_count);
				i++;
			}
		}
	}
	void PatternScaler::scaleInput(double* values) const {
		size_t i = 0;
		std::vector<Scaler*>::const_iterator it;
		for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
			values[i] = (*it)->scale(values[i]);
			i++;
		}
	}
	void PatternScaler::scaleInput(double* values, size_t startScaler, size_t numScalers) const {
		assert(startScaler+numScalers <= m_inputScalers.size());
		
		for (size_t i = 0; i < numScalers; ++i) {
			values[i] = m_inputScalers[startScaler+i]->scale(values[i]);
		}
	}
	void PatternScaler::scaleTargets(NPP2::PatternSet& patternSet) const {
		assert(patternSet.target_count == m_targetScalers.size());
		{
			size_t i = 0;
			std::vector<Scaler*>::const_iterator it;
			for (it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				(*it)->scale(patternSet.target, i, patternSet.pattern_count);
				i++;
			}
		}
	}
	
	double* PatternScaler::copyAndScaleInput(double const* input) const {
		double* result = new double[numInputDimensions()];
		size_t i = 0;
		std::vector<Scaler*>::const_iterator it;
		for (it = m_inputScalers.begin(); it != m_inputScalers.end(); it++) {
			result[i] = (*it)->scale(input[i]);
			i++;
		}
		return result;
	}
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Restoring the original values from scaled values
#endif
	/** \name Restoring the original values from scaled values
	 @{ */
	void PatternScaler::originalTargetValues(double* values) const {
		{
			size_t i = 0;
			std::vector<Scaler*>::const_iterator it;
			for (it = m_targetScalers.begin(); it != m_targetScalers.end(); it++) {
				values[i] = (*it)->originalValue(values[i]);
				i++;
			}
		}
	}
	
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#pragma mark Dimension informations
#endif
	/** \name Dimension informations
	 @{ */
	
	size_t PatternScaler::numInputDimensions() const {
		return m_inputScalers.size();
	}
	size_t PatternScaler::numTargetDimensions() const {
		return m_targetScalers.size();
	}
	
	/*@}*/
#ifdef __APPLE__
#pragma mark -
#endif
	
}
