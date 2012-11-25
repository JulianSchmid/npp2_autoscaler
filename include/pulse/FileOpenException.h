#ifndef micropulse_FileOpenException_h
#define micropulse_FileOpenException_h

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

#include <exception>
#include <string>

namespace pulse {

/** This expection is beeing thrown if the opening of a file has failed */
class FileOpenException : public std::exception {
public:
	/** Constructor
	 *  \param filename the file that could not be opend
	 */
	FileOpenException(const std::string& filename);
	FileOpenException(const FileOpenException& other);
	FileOpenException& operator= (const FileOpenException& other) throw();
	virtual ~FileOpenException() throw() {}
	virtual const char* what() const throw();
private:
	std::string m_filename;
	std::string m_whatStr;
};

}

#endif
