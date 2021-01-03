/* ***************************************************************************
 * DesignerOptions.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "DesignerOptions.hpp"

#include <getopt.h>
#include <cassert>
#include <ostream>
#include <iostream>
#include <cstring>
#include <sstream>

const char *const defaultOutputFilename = "out.mzm";

/*!
 * The option information.
 */
const Options::OptionsInfo designerOptionsInfo[] = {
	{"help",         no_argument,       0, 'h', 0,           "Print this help message"},
	{"version",      no_argument,       0, 'v', 0,           "Print version information"},
	{"level",        required_argument, 0, 'l', "levelno",   "Start with the specified level"},
	{"infile",       required_argument, 0, 'i', "infile",    "Load the level from this file"},
	{"monochrome",   no_argument,       0, 'm', 0,           "Don't use colors even if available"},
	{"threads",      required_argument, 0, 't', "numthreads","Number of threads to use"},
};

const int NUM_OF_DESIGNER_OPTIONS_INFO = sizeof(designerOptionsInfo) / sizeof(Options::OptionsInfo);

DesignerOptions::DesignerOptions() 
: Options(designerOptionsInfo
, NUM_OF_DESIGNER_OPTIONS_INFO)
, m_doHelp(false)
, m_doVersion(false)
, m_doOutputFile(false)
, m_monochrome(false)
, m_levelNumber(-1)
, m_numThreads(2)
{
}

void DesignerOptions::parseOptions(int argc, char* argv[])
{
	Options::parseOptions(argc,argv);
	errorIf (optind < argc - 1, "Too many parameters", argv[0]);
	if (optind == argc - 1) 
		m_outputFilename = argv[optind];
	else
		m_outputFilename = defaultOutputFilename;
}

void DesignerOptions::handleOption(char optionChar, const char* optarg, const char* pname, const char* optused)
{
	switch (optionChar) {
		case 'h':
			m_doHelp = true;
			break;
		case 'v':
			m_doVersion = true;
			break;
		case 'm':
			m_monochrome = true;
			break;
		case 'l': {
			std::stringstream arg(optarg);
			arg >> m_levelNumber;
			errorIf(arg.fail(),"Bad level number", optused, pname);
			errorIf(m_levelNumber < 1, "Level number must be greater or equal to one", optused, pname);
			break;
		}
		case 't': {
			std::stringstream arg(optarg);
			arg >> m_numThreads;
			errorIf(arg.fail(),"Bad thread number", optused, pname); 
			errorIf(m_numThreads < 2, "There must be two or more threads", optused, pname);
			break;
		}
		case 'i':
			errorIf (m_inputFilename != "", "Input file already selected", optused, pname);
			m_inputFilename = optarg;
			break;
		case '?':
		default:
			throw HandledBadArgument();
			break;
	}
}


bool DesignerOptions::isHelp() const
{
	return m_doHelp;
}

bool DesignerOptions::isVersion() const
{
	return m_doVersion;
}

bool DesignerOptions::isOutputFile() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_doOutputFile;
}

bool DesignerOptions::isMonochrome() const
{
	return m_monochrome;
}


const std::string& DesignerOptions::getInputFilename() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_inputFilename;
}

const std::string& DesignerOptions::getOutputFilename() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_outputFilename;
}

int DesignerOptions::getLevelNumber() const
{
	return m_levelNumber;
}

unsigned int DesignerOptions::getNumThreads() const
{
	return m_numThreads;
}


void DesignerOptions::printUsage(std::ostream& os, const char* pname) const
{
	os << "Usage: " << pname << " [-i infile] [-l n] [-t numthreads] [-m] [outfile]\n";
	os << "Starts with the last or nth file, in \"out.mzm\" or infile, if specified.\n";
	os << "Saves to \"out.mzm\" or outfile, if specified.\n";
}


void DesignerOptions::printExamples(std::ostream& os, const char* pname) const
{
	os << "Example: (Design a mazezam starting with level 9 in source.mzm and outputing to target.mzm.)\n";
	os << "$ " << pname << " -i source.mzm -l 9 -t 4 target.mzm\n";
}
