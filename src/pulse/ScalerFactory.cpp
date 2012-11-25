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

#include <pulse/ScalerFactory.h>

#include <pulse/Normalize.h>
#include <pulse/NormalizeWithFixpoint.h>

#include <pulse/CSVReader.h>

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace pulse {
	ScalerFactory::ScalerFactory(const std::string& file) : m_filePath(file) {
		
	}
	ScalerFactory::~ScalerFactory() {
		
	}
	size_t ScalerFactory::getMaxId(const std::string& prefix) throw (FileOpenException, ParseException) {
		size_t result = 0;
		CSVReader reader(m_filePath);
		//search id
		while (reader.good() && !false) {
			std::string currentId = reader.readEntry();
			if (currentId.size() > prefix.size()) {
				std::string nameStr = currentId.substr(0, prefix.size());
				if (nameStr.compare(prefix) == 0) {
					std::string numStr = currentId.substr(prefix.size(), currentId.size()-prefix.size());
					size_t num = atoi(numStr.c_str());
					if (num > result) {
						result = num;
					}
				}
			}
			if (reader.good()) {
				//jump over the other fields
				std::string t = reader.readEntry();
				if (t.compare(std::string("Normalize")) == 0) {
					std::vector<double> tmp;
					reader.readEntries(4, tmp);
				} else if (t.compare(std::string("NormalizeWithFixpoint")) == 0) {
					std::vector<double> tmp;
					reader.readEntries(6, tmp);
				}
			}
		}
		return result;
	}
	Scaler* ScalerFactory::getScaler(const std::string& id) throw (FileOpenException, ParseException) {
		CSVReader reader(m_filePath);
		//search id
		while (reader.good() && !false) {
			std::string currentId = reader.readEntry();
			if (currentId.compare(id) == 0) {
				Scaler* re = 0;
				//parse type
				std::string t = reader.readEntry();
				if (t.compare(std::string("Normalize")) == 0) {
					re = new Normalize(0.0,1.0);
					std::vector<double> tmp;
					reader.readEntries(4, tmp);
					re->setParameters(tmp);
					if (!reader.isAtLineStart()) {
						throw ParseException("too many parameters for Normalize");
					}
					return re;
				} else if (t.compare(std::string("NormalizeWithFixpoint")) == 0) {
					re = new NormalizeWithFixpoint(0.0, 0.0, -1.0, 1.0);
					std::vector<double> tmp;
					reader.readEntries(6, tmp);
					re->setParameters(tmp);
					if (!reader.isAtLineStart()) {
						throw ParseException("too many parameters for NormalizeWithFixpoint");
					}
					return re;
				}
				throw ParseException("Unknown scaler type");
			}
		}
		throw ParseException("Scaler not found");
	}
}