/* ***************************************************************************
 * Options.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <vector>

struct option;

/*!
 * Class which interprets the command line arguments.
 */
class Options
{
public:
	/*!
	 * Parse the ptions in argc and argv.
	 * \throw HandledBadArgument if the options cannot be correctly parsed.
	 * \param argc a count of the arguments.
	 * \param argv the arguments as null-ended character arrays.
	 */
	virtual void parseOptions(int argc, char* argv[]);

	/*!
	 * Exception thrown when the option parser has already printed a suitable
	 * error.
	 */
	class HandledBadArgument { };
	/*!
	 * Destructor.
	 */
	virtual ~Options();
	/*!
	 * Print out information about the flags.
	 * \param os the output stream.
	 * \param pname the name of the program.
	 */
	void printFlags(std::ostream& os, const char* pname) const;
	/*!
	 * Print out a usage message.
	 * \param os the output stream.
	 * \param pname the name of the program.
	 */
	virtual void printUsage(std::ostream& os, const char* pname) const = 0;
	/*!
	 * Print out example usage.
	 * \param os the output stream.
	 * \param pname the name of the program.
	 */
	virtual void printExamples(std::ostream& os, const char* pname) const = 0;

	/*!
	 * In order to keep short options, long options and their descriptions
	 * consistent, this is a single structure which contains all the
	 * option information.
	 * Currently this supports options which have both a long and a short form.
	 */
	struct OptionsInfo {
		/*! \see getopt documentation. */
		const char* name;
		/*! \see getopt documentation. */
		int has_arg;
		/*! \see getopt documentation. */
		int* flag;
		/*! \see getopt documentation. */
		int val;
		/*! A name for the argument to print when giving help. */
		const char* argumentName;
		/*! A description of the option to print when giving help. */
		const char* description;
	};
protected:
	
	const OptionsInfo* m_optionsInfo;
	
	int m_numOfOptionsInfo;
	
	/*!
	 * Constructor.
	 */
	Options(const OptionsInfo* optionsInfo, int numOfOptionsInfo);
	
	/*!
	 * Override this with handling code.
	 */
	virtual void handleOption(char optionChar, const char* optarg, const char* pname, const char* optused) = 0;
	
	/*!
	 * Print out an error message and throws an exception if the test is true.
	 * \param b the test.
	 * \param message the message.
	 * \param pname the name of the program.
	 */
	void errorIf(bool b, const char* message, const char* pname) const;
	/*!
	 * Print out an error message for option opt and throws an exception if the test is true.
	 * \param b the test.
	 * \param message the message.
	 * \param opt the option.
	 * \param pname the name of the program
	 */
	void errorIf(bool b, const char* message, const char* opt, const char* pname) const;

private:
	/*!
	 * Builds an array of long options.
	 */
	void buildLongOptions(std::vector<option>& longOptions) const;
	/*!
	 * Builds a string of short options.
	 */
	void buildShortOptions(std::string& shortOptions) const;
	/*!
	 * Return the width of printing the left-hand column in a flags description.
	 * \return the width of printing the left-hand column in a flags description.
	 * \param i the entry for this option in the mixed_options array. 
	 */
	unsigned int lineSize(int i) const;
};

#endif /*OPTIONS_H_*/
