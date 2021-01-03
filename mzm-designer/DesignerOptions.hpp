/* ***************************************************************************
 * DesignerOptions.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef DESIGNEROPTIONS_H_
#define DESIGNEROPTIONS_H_

#include <string>

#include <mzmcommon/Options/Options.hpp>

/*!
 * Class which interprets the command line arguments for the designer program.
 */
class DesignerOptions : public Options
{
public:
	/*!
	 * Constructor.
	 */
	DesignerOptions();
	/*!
	 * Return true iff we should print out a help message.
	 */
	bool isHelp() const;
	/*!
	 * Return true iff we should print out a version message.
	 */
	bool isVersion() const;
	/*!
	 * Return true iff the user has specified an output file.
	 */
	bool isOutputFile() const;
	/*!
	 * Return true iff the user selected monochrome.
	 */
	bool isMonochrome() const;
	/*!
	 * Returns the input filename.
	 */
	const std::string& getInputFilename() const;
	/*!
	 * Returns the output filename.
	 */
	const std::string& getOutputFilename() const;
	/*!
	 * Returns the level number.
	 */
	int getLevelNumber() const;
	/*!
	 * Returns the number of threads to use. 
	 */
	unsigned int getNumThreads() const;

	virtual void parseOptions(int argc, char* argv[]);
	virtual void printUsage(std::ostream& os, const char* pname) const;
	virtual void printExamples(std::ostream& os, const char* pname) const;
	
protected:
	virtual void handleOption(char optionChar, const char* optarg, const char* pname, const char* optused);
public:
	/*!
	 * Whether we should output a help message.
	 */
	bool m_doHelp;
	/*!
	 * Whether we should output a version message.
	 */
	bool m_doVersion;

	/*!
	 * Whether an output file has been specified.
	 */
	bool m_doOutputFile;
	/*!
	 * Don't use colors, even if available. 
	 */
	bool m_monochrome;
	/*!
	 * The input file, if there was one.
	 */
	std::string m_inputFilename;
	/*!
	 * The output file, if there was one.
	 */
	std::string m_outputFilename;
	/*!
	 * The level number to be opened (can be strictly positive or -1).
	 */
	int m_levelNumber;
	/*!
	 * The number of threads to use.
	 */
	unsigned int m_numThreads;
};

#endif /*DESIGNEROPTIONS_H_*/
