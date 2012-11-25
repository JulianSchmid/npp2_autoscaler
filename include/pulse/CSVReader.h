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

#include <fstream>
#include <vector>
#include <cstddef>

#include <pulse/FileOpenException.h>
#include <pulse/ParseException.h>

namespace pulse{
	/** Reads very simple csv files.
	 *  \warning this not a real complete csv reader, there is no support for the seperator character or newlines in a string or quotes.
	 */
	class CSVReader {
	public:
		/** Opens a file to read it
		 * \param filename
		 * \param seperator
		 */
		CSVReader(const std::string& filename, char seperator='\t') throw (FileOpenException);
		/** Deconstructor - closes the filestream */
		virtual ~CSVReader();
		
		/** Reads a line that only contains double values 
		 * \param re the result
		 */
		void readLine(std::vector<double>& re) throw (ParseException);
		/** Reads a string */
		std::string readEntry() throw (ParseException);
		/** Reads a number of string values
		 *  \pre num > 0
		 *  \param num the number of strings that should be read
		 *  \param re the result
		 */
		void readEntries(size_t num, std::vector<std::string>& re) throw (ParseException);
		/** Reads a number of double values
		 *  \pre num > 0
		 *  \param num the number of doubles that should be read
		 *  \param re the result
		 */
		void readEntries(size_t num, std::vector<double>& re) throw (ParseException);
		/** Goes to the next line, if at the beginning !good() an exception is thrown
		 */
		void goToNextLine() throw (ParseException);
		/** Returns if more data can be read from the stream */
		bool good() const;
		/** True if there were no values read so far */
		bool isAtLineStart() const;
	private:
		std::ifstream m_fstream;
		char m_seperator;
		size_t m_char;
	};
}