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

#include <pulse/NormalizeWithFixpoint.h>

#include <cassert>
#include <iostream>

namespace pulse {
	std::string NormalizeWithFixpoint::m_name = std::string("NormalizeWithFixpoint");
	
	NormalizeWithFixpoint::NormalizeWithFixpoint(double fixpoint, double fixpointNorm, double minNorm, double maxNorm) :
		m_min(fixpoint-1.0),
		m_max(fixpoint+1.0),
		m_fixpoint(fixpoint),
		m_fixpointNorm(fixpointNorm),
		m_minNorm(minNorm),
		m_maxNorm(maxNorm)
	{
		assert(m_minNorm < m_maxNorm);
		assert(m_min < m_max);
		assert(m_minNorm < m_fixpointNorm);
		assert(m_maxNorm > m_fixpointNorm);
		assert(m_min < m_fixpoint);
		assert(m_max > m_fixpoint);
	}
	NormalizeWithFixpoint::NormalizeWithFixpoint(double fixpoint, double fixpointNorm, double minNorm, double maxNorm, double seenMin, double seenMax) :
		m_min(seenMin),
		m_max(seenMax),
		m_fixpoint(fixpoint),
		m_fixpointNorm(fixpointNorm),
		m_minNorm(minNorm),
		m_maxNorm(maxNorm)
	{
		assert(m_minNorm < m_maxNorm);
		assert(m_min < m_max);
		assert(m_minNorm < m_fixpointNorm);
		assert(m_maxNorm > m_fixpointNorm);
		assert(m_min < m_fixpoint);
		assert(m_max > m_fixpoint);
	}
	void NormalizeWithFixpoint::updateScalingFactors(const double* data, size_t offset, size_t num) {
		assert(num > 0);
		//determine min and max
		m_min = m_fixpoint;
		m_max = m_fixpoint;
		for (int i = 0; i < num; i++) {
			if (m_max < data[i*offset]) {
				m_max = data[i*offset];
			}
			if (m_min > data[i*offset]) {
				m_min = data[i*offset];
			}
		}

		//make sure that the post condition is met
		if (m_max - m_fixpoint == 0.0) {
			std::cerr<<"m_fixpoint was == m_max"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}
		if (m_fixpoint - m_min == 0.0) {
			std::cerr<<"m_fixpoint was == m_min"<<std::endl;
			m_min = m_min - m_fixpoint*m_fixpoint  - 1.0;
		}

		//post condition
		assert(m_min < m_max);
		assert(m_min < m_fixpoint);
		assert(m_max > m_fixpoint);
	}
	void NormalizeWithFixpoint::updateScalingFactors(double** const data, size_t offset, size_t num) {
		for (size_t i = 0; i < num; i++) {
			if (m_max < data[i][offset]) {
				m_max = data[i][offset];
			}
			if (m_min > data[i][offset]) {
				m_min = data[i][offset];
			}
		}
		//make sure that the post condition is met
		if (m_max - m_fixpoint == 0.0) {
			std::cerr<<"m_fixpoint was == m_max"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}
		if (m_fixpoint - m_min == 0.0) {
			std::cerr<<"m_fixpoint was == m_min"<<std::endl;
			m_min = m_min - m_fixpoint*m_fixpoint  - 1.0;
		}

		//post condition
		assert(m_min < m_max);
		assert(m_min < m_fixpoint);
		assert(m_max > m_fixpoint);
	}
	void NormalizeWithFixpoint::updateScalingFactors(double value) {
		if (m_max < value) {
			m_max = value;
		}
		if (m_min > value) {
			m_min = value;
		}
		//make sure that the post condition is met
		if (m_max - m_fixpoint == 0.0) {
			std::cerr<<"m_fixpoint was == m_max"<<std::endl;
			m_max = m_max + m_max*m_max  + 1.0;
		}
		if (m_fixpoint - m_min == 0.0) {
			std::cerr<<"m_fixpoint was == m_min"<<std::endl;
			m_min = m_min - m_fixpoint*m_fixpoint  - 1.0;
		}
		
		//post condition
		assert(m_min < m_max);
		assert(m_min < m_fixpoint);
		assert(m_max > m_fixpoint);
	}
	void NormalizeWithFixpoint::resetScalingFactors(double const* data, size_t offset, size_t num) {
		assert(num > 0);
		m_min = m_fixpoint;
		m_max = m_fixpoint;
		updateScalingFactors(data, offset, num);
	}
	void NormalizeWithFixpoint::resetScalingFactors(double** const data, size_t offset, size_t num) {
		assert(num > 0);
		m_min = m_fixpoint;
		m_max = m_fixpoint;
		updateScalingFactors(data, offset, num);
	}
	void NormalizeWithFixpoint::scale(const double* in, int inOffset, double* out, size_t outOffset, size_t num) const {
		for (int i = 0; i < num; i++) {
			out[i*outOffset] = scale(in[i*inOffset]);
		}
	}
	void NormalizeWithFixpoint::scale(double** data, size_t offset, size_t num) const {
		for (size_t i = 0; i < num; i++) {
			data[i][offset] = scale(data[i][offset]);
		}
	}
	double NormalizeWithFixpoint::scale(double value) const {
		double re = 0.0;
		if (value < m_fixpoint) {
			// [m_min, m_fixpoint) -> [0,1) ...min<----f...
			double normFixDist = (m_fixpoint-value)/(m_fixpoint-m_min);
			// [0,1) -> [m_norMin,m_normFixpoint)
			re = m_fixpointNorm-((m_fixpointNorm-m_minNorm)*normFixDist);
		} else {
			// [m_fixpoint,m_max] -> [0,1] f---->max....]
			double normFixDist = (value-m_fixpoint)/(m_max-m_fixpoint);
			// [0,1] -> [m_normFixpoint,m_normMax]
			re = m_fixpointNorm + ((m_maxNorm-m_fixpointNorm)*normFixDist);
		}
		if (re+0.00000000001 < m_minNorm) {
			std::cerr<<"got value that resulted in a value that was smaller than the m_minNorm"<<std::endl;
			return re;
		} else if (re > m_maxNorm) {
			std::cerr<<"got value that resulted in a value that was bigger than the m_maxNorm"<<std::endl;
			return re;
		} else {
			return re;
		}
	}
	double NormalizeWithFixpoint::originalValue(double value) const {
		if (value < m_fixpointNorm) {
			//[m_minNorm, m_fixpointNorm) -> [0,1)   ...maxN<----fNorm....
			double normFixDist = (m_fixpointNorm-value)/(m_fixpointNorm-m_minNorm);
			//[0,1) -> [m_min,m_fixpoint)
			return m_fixpoint -((m_fixpoint-m_min)*normFixDist);
		} else {
			//[m_fixpointNorm, m_maxNorm] -> [0,1]   ...fNorm---->minN...
			double normFixDist = (value-m_fixpointNorm)/(m_maxNorm-m_fixpointNorm);
			//[0,1] -> [m_fixpoint,m_max]
			return m_fixpoint + ((m_max-m_fixpoint)*normFixDist);
		}
	}
	void NormalizeWithFixpoint::getParameters(std::vector<double>& params) const {
		assert(params.empty());
		params.push_back(m_min);
		params.push_back(m_max);
		params.push_back(m_fixpoint);
		params.push_back(m_fixpointNorm);
		params.push_back(m_minNorm);
		params.push_back(m_maxNorm);
	}
	void NormalizeWithFixpoint::setParameters(const std::vector<double>& params) {
		assert(params.size() == 6);
		m_min = params[0];
		m_max = params[1];
		m_fixpoint = params[2];
		m_fixpointNorm = params[3];
		m_minNorm = params[4];
		m_maxNorm = params[5];
	}
	const std::string& NormalizeWithFixpoint::getTypeName() const {
		return m_name;
	}
	 Scaler* NormalizeWithFixpoint::clone() const {
		return new NormalizeWithFixpoint(m_fixpoint, m_fixpointNorm, m_minNorm, m_maxNorm, m_min, m_max);
	 }
}