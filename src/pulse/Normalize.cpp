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

#include <pulse/Normalize.h>

#include <iostream>
#include <cassert>

namespace pulse {
	std::string Normalize::m_name = std::string("Normalize");
	
	Normalize::Normalize(double minNorm, double maxNorm) :
		m_min(0.0),
		m_max(1.0),
		m_minNorm(minNorm),
		m_maxNorm(maxNorm)
	{
		//pre conditions
		assert(minNorm < maxNorm);
		//post condition
		assert(m_minNorm < m_maxNorm);
		assert(m_min == 0.0);
		assert(m_max == 1.0);
	}
	Normalize::Normalize(double minNorm, double maxNorm, double seenMin, double seenMax) :
		m_min(seenMin),
		m_max(seenMax),
		m_minNorm(minNorm),
		m_maxNorm(maxNorm)
	{
		//preconditions
		assert(minNorm < maxNorm);
		assert(seenMin < seenMax);
		//postconditions
		assert(m_minNorm < m_maxNorm);
		assert(m_min < m_max);
	}
	void Normalize::updateScalingFactors(double const* data, size_t offset, size_t num) {
		assert(num > 0);
		//determine min and max
		for (int i = 1; i < num; i++) {
			if (m_max < data[i*offset]) {
				m_max = data[i*offset];
			}
			if (m_min > data[i*offset]) {
				m_min = data[i*offset];
			}
		}
		//make sure that the post condition is met
		if (m_max - m_min == 0.0) {
			std::cerr<<"currentMaxQ was == currentMinQ"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}

		//post condition
		assert(m_min < m_max);
	}
	void Normalize::updateScalingFactors(double** const data, size_t offset, size_t num) {
		for (size_t i = 0; i < num; i++) {
			if (m_max < data[i][offset]) {
				m_max = data[i][offset];
			}
			if (m_min > data[i][offset]) {
				m_min = data[i][offset];
			}
		}
		//make sure that the post condition is met
		if (m_max - m_min == 0.0) {
			std::cerr<<"currentMaxQ was == currentMinQ"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}

		//post condition
		assert(m_min < m_max);
	}
	void Normalize::updateScalingFactors(double value) {
		if (m_max < value) {
			m_max = value;
		}
		if (m_min > value) {
			m_min = value;
		}
		//make sure that the post condition is met
		if (m_max - m_min == 0.0) {
			std::cerr<<"currentMaxQ was == currentMinQ"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}
	}
	void Normalize::resetScalingFactors(double const* data, size_t offset, size_t num) {
		assert(num > 0);
		m_min = std::numeric_limits<double>::infinity();
		m_max = std::numeric_limits<double>::infinity()*-1.0;
		updateScalingFactors(data, offset, num);
	}
	void Normalize::resetScalingFactors(double** const data, size_t offset, size_t num) {
		assert(num > 0);
		m_min = std::numeric_limits<double>::infinity();
		m_max = std::numeric_limits<double>::infinity()*-1.0;
		updateScalingFactors(data, offset, num);
	}
	void Normalize::scale(double const* in, int inOffset, double* out, size_t outOffset, size_t num) const {
		for (int i = 0; i < num; i++) {
			out[i*outOffset] = scale(in[i*inOffset]);
		}
	}
	double Normalize::scale(double value) const {
		double re = ((value-m_min)/(m_max-m_min))*(m_maxNorm-m_minNorm)+m_minNorm;
		if (re < m_minNorm) {
			std::cerr<<"got value that was smaller than the m_minNorm re="<<re<<" m_minNorm="<<m_minNorm<<std::endl;
			return re;
		} else if (re > m_maxNorm) {
			std::cerr<<"got value that was bigger than the m_maxNorm"<<std::endl;
			return re;
		} else {
			return re;
		}
	}
	void Normalize::scale(double** data, size_t offset, size_t num) const {
		for (size_t i = 0; i < num; i++) {
			data[i][offset] = scale(data[i][offset]);
		}
	}
	double Normalize::originalValue(double value) const {
		return ((value - m_minNorm)/(m_maxNorm-m_minNorm))*(m_max-m_min) + m_min;
	}
	void Normalize::getParameters(std::vector<double>& params) const {
		assert(params.empty());
		params.push_back(m_min);
		params.push_back(m_max);
		params.push_back(m_minNorm);
		params.push_back(m_maxNorm);
	}
	void Normalize::setParameters(const std::vector<double>& params) {
		assert(params.size() == 4);
		m_min = params[0];
		m_max = params[1];
		m_minNorm = params[2];
		m_maxNorm = params[3];
	}
	
	const std::string& Normalize::getTypeName() const {
		return m_name;
	}
	
	Scaler* Normalize::clone() const {
		return new Normalize(m_minNorm, m_maxNorm, m_min, m_max);
	}
}