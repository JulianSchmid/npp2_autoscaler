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

#include <pulse/Scaler.h>

namespace pulse {
	/**
	 * \brief The class Normalize linearly scales the values down or up to two boundry values m_minNorm and m_maxNorm based on the seen maximum and minimum values in the data. To communicate these values you can use the updateScalingFactors and resetScalingFactors methods.
	 */
	class Normalize : public Scaler {
	public:
		/** Constructor, 
		 *  \note if you use this constructor you have to make sure, that you call the reset/update oparation at least once to correctly set the m_min and m_max values.
		 *  \pre minNorm < maxNorm
		 *  \post m_minNorm < m_maxNorm
		 *  \post m_min == 0.0
		 *  \post m_max == 1.0
		 *  \param minNorm the minimal double value of a scaled value
		 *  \param maxNorm the maximal double value of a scaled value
		 */
		Normalize(double minNorm, double maxNorm);
		/** Constructor
		 *  \pre minNorm < maxNorm
		 *  \pre seenMin < seenMax
		 *  \post m_minNorm < m_maxNorm
		 *  \post m_min < m_max
		 *  \param minNorm the minimal double value of all scaled values
		 *  \param maxNorm the maximal double value of all scaled values
		 *  \param seenMin the minimal double value of all not scaled values (can/will be automatically changed by calling updateScalingFactors or resetScalingFactors methods)
		 *  \param seenMax the maximal double value of all not scaled values (can/will be automatically changed by calling updateScalingFactors or resetScalingFactors methods)
		 */
		Normalize(double minNorm, double maxNorm, double seenMin, double seenMax);
		virtual ~Normalize() {}
		virtual void updateScalingFactors(double const* data, size_t offset, size_t num);
		virtual void updateScalingFactors(double** const data, size_t offset, size_t num);
		virtual void updateScalingFactors(double value);
		virtual void resetScalingFactors(double const* data, size_t offset, size_t num);
		virtual void resetScalingFactors(double** const data, size_t offset, size_t num);
		virtual void scale(double const* in, int inOffset, double* out, size_t outOffset, size_t num) const;
		virtual void scale(double** data, size_t offset, size_t num) const;
		virtual double scale(double value) const;
		virtual double originalValue(double value) const;
		virtual void getParameters(std::vector<double>& params) const;
		virtual void setParameters(const std::vector<double>& params);
		virtual const std::string& getTypeName() const;
		virtual Scaler* clone() const;
	private:
		double m_min;
		double m_max;
		double m_minNorm;
		double m_maxNorm;
		static std::string m_name;
	};
}