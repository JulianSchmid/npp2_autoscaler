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

#include <pulse/CSVReader.h>

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

namespace pulse {

	CSVReader::CSVReader(const std::string& filename, char seperator) throw(FileOpenException) : m_seperator(seperator), m_char(0) {
		m_fstream.open(filename.c_str());
		if (!m_fstream.good()) {
			throw FileOpenException(filename);
		}
	}
	CSVReader::~CSVReader() {
		m_fstream.close();
	}
	void CSVReader::readLine(std::vector<double>& re) throw (ParseException) {
		if (!m_fstream.good()) {
			throw ParseException("unexpected end of file reached");
		}
		if (m_char != 0) {
			throw ParseException("unexpected parsing of not ended field");
		}
	
		bool done = false;
		std::stringstream s;
		while(m_fstream.good() && !done) {
			char curr = m_fstream.get();
			if (curr != '\n' && curr != m_seperator && m_fstream.good()) {
				s<<curr;
			} else {
				re.push_back(atof(s.str().c_str()));
				s.str(std::string());
				if (curr == '\n' || !m_fstream.good()) {
					done = true;
				}
			}
		}
	}
	
	std::string CSVReader::readEntry() throw (ParseException) {
		if (!m_fstream.good()) {
			throw ParseException("unexpected end of file reached");
		}
		
		std::stringstream s;
		while(m_fstream.good()) {
			char curr = m_fstream.get();
			if (curr != '\n' && curr != m_seperator && m_fstream.good()) {
				s<<curr;
			} else {
				if (curr == '\n') {
					m_char = 0;
				}
				break;
			}
			m_char++;
		}
		return s.str();
	}
	
	void CSVReader::readEntries(size_t num, std::vector<std::string>& re) throw (ParseException) {
		if (!m_fstream.good()) {
			throw ParseException("unexpected end of file reached");
		}
		assert(num > 0);
		
		for (size_t i = 0; i < num; i++) {
			bool done = false;
			std::stringstream s;
			while(m_fstream.good() && !done) {
				char curr = m_fstream.get();
				if (curr != '\n' && curr != m_seperator && m_fstream.good()) {
					s<<curr;
				} else {
					if (!(curr != '\n' || i+1 == num)) {
						throw ParseException("unexpected end of line reached");
					}
					re.push_back(s.str());
					done = true;
				}
				m_char++;
				if (curr == '\n') {
					m_char = 0;
				}
			}
		}
	}
	void CSVReader::readEntries(size_t num, std::vector<double>& re) throw (ParseException) {
		assert(num > 0);
		std::vector<std::string> tmp;
		readEntries(num, tmp);
		std::vector<std::string>::const_iterator it;
		for (it = tmp.begin(); it != tmp.end(); it++) {
			re.push_back(atof(it->c_str()));
		}
	}
	void CSVReader::goToNextLine() throw (ParseException) {
		if (!m_fstream.good()) {
			throw ParseException("unexpected end of file reached");
		}
		while(m_fstream.good()) {
			char curr = m_fstream.get();
			if (curr == '\n') {
				break;
			}
		}
		m_char = 0;
	}
	bool CSVReader::isAtLineStart() const {
		return (m_char == 0);
	}

	bool CSVReader::good() const {
		return m_fstream.good();
	}

}
