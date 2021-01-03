/* ***************************************************************************
 * SolverOptions.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SOLVEROPTIONS_H_
#define SOLVEROPTIONS_H_

#include <string>

#include <mzmcommon/Options/Options.hpp>
#include <mzm/RangePred/RangePred.hpp>

/*!
 * Class which interprets the command line arguments.
 */
class SolverOptions : public Options
{
public:
	/*!
	 * What is the source of levels?
	 */
	enum sourceMode {
		/*! use the defaults */
		DEFAULTS,
		/*! use an input file */
		INPUT_FILE,
		/*! Use std::cin */
		STDIN
	};

public:
	SolverOptions();
	/*!
	 * Construct the Options class with argc and argv.
	 * \throw HandledBadArgument if the options cannot be correctly parsed.
	 * \param argc a count of the arguments.
	 * \param argv the arguments as null-ended character arrays.
	 */
	virtual void parseOptions(int argc, char* argv[]);
	
	virtual void handleOption(char optionChar, const char* optarg, const char* pname, const char* optused);
	/*!
	 * Return true iff we should print out a help message.
	 */
	bool isHelp() const;
	/*!
	 * Return true iff we should print out a version message.
	 */
	bool isVersion() const;
	/*!
	 * Return true iff the input file should be copied.
	 */
	bool isCopyMode() const;
	/*!
	 * Return true iff a range has been specified.
	 */
	bool isRange() const;
	/*!
	 * Return true iff the user has specified an output file.
	 */
	bool isOutputFile() const;
	/*!
	 * Return true iff the user has asked for A*.
	 */
	bool isAStar() const;
	/*!
	 * Return the source of the levels.
	 * \return the source of the levels.
	 */
	sourceMode getSourceMode() const;
	/*!
	 * Return what kind of solutions are required.
	 */
	unsigned int getSolutionFlags() const;
	/*!
	 * Returns the input filename.
	 */
	const std::string& getInputFilename() const;
	/*!
	 * Returns the output filename.
	 */
	const std::string& getOutputFilename() const;
	/*!
	 * Return the range predicate.
	 */
	const RangePred& getLevelSpec() const;
	/*!
	 * 
	 */
	unsigned int getNumThreads() const;
	/*!
	 * Print out a usage message.
	 * \param os the output stream.
	 * \param pname the name of the program.
	 */
	virtual void printUsage(std::ostream& os, const char* pname) const;
	/*!
	 * Print out example usage.
	 * \param os the output stream.
	 * \param pname the name of the program.
	 */
	virtual void printExamples(std::ostream& os, const char* pname) const;
private:
	/*!
	 * Whether we should output a help message.
	 */
	bool m_doHelp;
	/*!
	 * Whether we should output a version message.
	 */
	bool m_doVersion;
	/*!
	 * Whether to copy the input file to the output file.
	 */
	bool m_doCopyMode;
	/*!
	 * Whether a range has been specified.
	 */
	bool m_doLevelSpec;
	/*!
	 * Whether to give a rating or not. 
	 */
	bool m_doRating;
	/*!
	 * Whether an output file has been specified.
	 */
	bool m_doOutputFile;
	/*!
	 * Whether to use A*.
	 */
	bool m_doAStar;
	/*!
	 * Specifies the source of levels.
	 */
	sourceMode m_sourceMode;
	/*!
	 * Specifies the kind of output we want.
	 */
	unsigned int m_solutionFlags;
	/*!
	 * The input file, if there was one.
	 */
	std::string m_inputFilename;
	/*!
	 * The output file, if there was one.
	 */
	std::string m_outputFilename;
	/*!
	 * The range predicate. 
	 */
	RangePred m_levelSpec;
	/*!
	 * 
	 */
	unsigned int m_numThreads;
};

#endif /*SOLVEROPTIONS_H_*/
