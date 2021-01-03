/* ***************************************************************************
 * FileMzmReader.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef FILEMZMREADER_H_
#define FILEMZMREADER_H_

#include <istream>

#include <mzm/Mazezam/MazezamData.hpp>
#include <mzm/MzmReaders/MzmReader.hpp>

namespace mzm {

/*!
 * An MzmReader which parses .mzm files from a istream and copies their non-level
 * contents to an ostream.
 */
class FileMzmReader : public MzmReader
{
public:
 	/*!
 	 * Construct a FileMzmReader to read mazezams from the given input stream.
 	 * \param is the input stream to read mazezams from.
 	 * \param os the output stream to copy the non-level contents to (possibly a DummyOStream).
 	 * \param fileName the name of the file, which can be empty e.g. for stdin.
 	 */
	FileMzmReader(std::istream& is, const std::string& fileName, std::ostream& os);
	/*!
	 * Thrown by next if there was a parsing error.
	 */
	class FileParseError : public ParseError {
		public:
			/*!
			 * Constructor.
			 */
			FileParseError (int line, const std::string& fileName):	ParseError(line), m_fileName(fileName) {}
			/*!
			 * Write details of the error to a stream.
			 */
			virtual void writeToStream (std::ostream& os) { os << m_fileName << ": "; ParseError::writeToStream(os); } 
		protected:
			const std::string m_fileName;
	};
	/*!
	 * Returns a copy of last level parsed.
	 * The caller must delete it.
	 */
	virtual MazezamData* getLevel() const;
	
	/*!
	 * Returns the level number of the last level parsed.
	 */
	virtual int getLevelNumber() const;
	
	/*!
	 * Attempt to parse the next level, return true if it succeeds.
	 */
	virtual bool next();
	
	/*!
	 * Copy the current level to the output stream, if one was provided.
	 */
	virtual void copyLevel();
	
	/*!
	 * Destructor.
	 */
	virtual ~FileMzmReader();
private:
	/*!
	 * The stream this reader is processing.
	 */
	std::istream& m_istream;
	/*!
	 * The stream this reader is copying to.
	 */
	std::ostream& m_ostream;
	/*!
	 * The filename, which can be empty e.g. for stdin.
	 */
	const std::string m_fileName;
	/*!
	 * The line number (relative to when this object was constructed).
	 */
	int m_lineNumber;
	/*!
	 * Points to a mzm_level if one was successfully parsed.
	 */
	MazezamData* m_level;
	/*!
	 * True if a level has been parsed.
	 */
	bool m_hasLevel;
	/*!
	 * The number of the last level in the file.
	 */
	int m_number;
	/*!
	 * Look through the file for a solid wall.
	 * \param width a reference used to return the width of the wall found.
	 * \return true if we found a solid wall.
	 * \throw ParseError if there is a problem. 
	 */
	bool findNextMazezam(mzm_coord& width);
	/*!
	 * Parse the rest of the mazezam found with findNextMazezam.
	 * \param width the expected width of the mazezam.
	 * \param height used to return the height of the mazezam.
	 * \param start used to return the row on which the start lies.
	 * \param finish used to return the row on which the finish lies.
	 * \throw ParseError if there is a problem. 
	 */
	void parseRestOfMazezam(MazezamData::array_type& block, mzm_coord width, mzm_coord& height, mzm_coord& start, mzm_coord& finish);
	/*!
	 * A simple test which throws an appropriate parse error.
	 * \param b the error is thrown if b is true.
	 */
	inline void errorIf (bool b) {
		if (b)
			throw FileParseError(m_lineNumber, m_fileName);
	}
	
	/*!
	 * The copy constructor is private and unimplemented.
	 */
	FileMzmReader(const FileMzmReader& r);
	
	/*!
	 * The assignment operator is private and unimplemented.
	 */
	FileMzmReader& operator=(const FileMzmReader& r);
};

std::ostream& operator<<(std::ostream& os, FileMzmReader::FileParseError e);

} // namespace mzm

#endif /*FILEMZMREADER_H_*/
