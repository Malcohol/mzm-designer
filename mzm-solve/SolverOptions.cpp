/* ***************************************************************************
 * SolverOptions.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "SolverOptions.hpp"

#include <getopt.h>
#include <cassert>
#include <ostream>
#include <iostream>
#include <cstring>
#include <sstream>

#include <mzm/MazezamSolver/MazezamSolutionType.hpp>

using namespace mzm;

/*!
 * The option information.
 */
const Options::OptionsInfo solverOptionsInfo[] = {
	{"help",         no_argument,       0, 'h', 0,          "Print this help message"},
	{"version",      no_argument,       0, 'v', 0,          "Print version information"},
	{"any-solution", no_argument,       0, 'a', 0,          "Find any solution"},
	{"pushes-only",  no_argument,       0, 'p', 0,          "Only find solution with fewest pushes"},
	{"moves-only",   no_argument,       0, 'm', 0,          "Only find solution with fewest moves"},
	{"both",         no_argument,       0, 'b', 0,          "Give both optimal solutions (default)"},
	{"copy",         no_argument,       0, 'c', 0,          "Copy input to output"},
	{"defaults",     no_argument,       0, 'd', 0,          "Use built-in default levels"},
	{"rating",       no_argument,       0, 'r', 0,          "Rate the levels (experimental)"},
	{"a-star",       no_argument,       0, 'A', 0,          "Use an A* algorithm (experimental)"},
	{"levels",       required_argument, 0, 'l', "levelspec","Only solve specified levels"},
	{"output",       required_argument, 0, 'o', "outfile",   "Write output to outfile"},
	{"threads",      required_argument, 0, 't', "numthreads","Number of threads to use"},
};

/*!
 * The number of options.
 */
const int NUM_OF_SOLVER_OPTIONS_INFO = sizeof(solverOptionsInfo) / sizeof(Options::OptionsInfo);


SolverOptions::SolverOptions()
: Options(&solverOptionsInfo[0], NUM_OF_SOLVER_OPTIONS_INFO)
, m_doHelp(false)
, m_doVersion(false)
, m_doCopyMode(false)
, m_doLevelSpec(false)
, m_doRating(false)
, m_doOutputFile(false)
, m_doAStar(false)
, m_sourceMode(STDIN)
, m_solutionFlags(0)
, m_numThreads(2)
{
}

void SolverOptions::parseOptions(int argc, char* argv[])
{
	Options::parseOptions(argc,argv);
	if (m_solutionFlags == 0)
		m_solutionFlags = MAZEZAM_SOLUTION_FEWEST_MOVES | MAZEZAM_SOLUTION_FEWEST_PUSHES;
	errorIf (optind < argc - 1, "Two many input files", argv[0]);
	if (optind < argc) {
		errorIf (m_sourceMode == DEFAULTS, "Cannot have -d and input file", argv[0]);
		m_sourceMode = INPUT_FILE;
		m_inputFilename = argv[optind];
	}
}

void SolverOptions::handleOption(char optionChar, const char* optarg, const char* pname, const char* optused)
{
	switch (optionChar) {
		case 'h':
			m_doHelp = true;
			break;
		case 'v':
			m_doVersion = true;
			break;
		case 'r':
			m_solutionFlags |= MAZEZAM_SOLUTION_RATING;
			break;
		case 'd':
			errorIf(m_doCopyMode, "Cannot copy defaults", optused, pname);
			m_sourceMode = DEFAULTS;
			break;
		case 'c':
			errorIf(m_sourceMode == DEFAULTS, "Cannot copy defaults",optused, pname);
			m_doCopyMode = true;
			break;
		case 'p':
			errorIf ((m_solutionFlags & MAZEZAM_SOLUTION_SEARCHES) & ~MAZEZAM_SOLUTION_FEWEST_PUSHES, "Cannot also select fewest pushes", optused, pname);
			m_solutionFlags |= MAZEZAM_SOLUTION_FEWEST_PUSHES;  
			break;
		case 'm':
			errorIf ((m_solutionFlags & MAZEZAM_SOLUTION_SEARCHES) & ~MAZEZAM_SOLUTION_FEWEST_MOVES, "Cannot also select fewest moves", optused, pname);
			m_solutionFlags |= MAZEZAM_SOLUTION_FEWEST_MOVES;
			break;
		case 'a':
			errorIf ((m_solutionFlags & MAZEZAM_SOLUTION_SEARCHES) & ~MAZEZAM_SOLUTION_FASTEST, "Cannot also select any solution", optused, pname);
			m_solutionFlags |= MAZEZAM_SOLUTION_FASTEST;
			break;
		case 'b':
			errorIf ((m_solutionFlags & MAZEZAM_SOLUTION_SEARCHES) & ~(MAZEZAM_SOLUTION_FEWEST_MOVES | MAZEZAM_SOLUTION_FEWEST_PUSHES), "Cannot also select any both", optused, pname);
			m_solutionFlags |= MAZEZAM_SOLUTION_FEWEST_MOVES | MAZEZAM_SOLUTION_FEWEST_PUSHES;
			break;
		case 'l':
			m_doLevelSpec = true;
			try {
				m_levelSpec.newArgument(optarg);
			} catch (RangePred::BadArgument e) {
				errorIf(true,"Bad level specification", optused, pname);
			}
			break;
		case 'o':
			errorIf (m_outputFilename != "", "Output file already selected", optused, pname);
			m_outputFilename = optarg;
			m_doOutputFile = true;
			break;
		case 'A':
			m_doAStar = true;
			break;
		case 't': {
			std::stringstream arg(optarg);
			int num;
			arg >> num;
			errorIf(arg.fail(),"Bad thread number", optused, pname); 
			errorIf(num < 1, "There must be at least one thread", optused, pname);
			m_numThreads = num;
			break;
		}
		case '?':
		default:
			throw HandledBadArgument();
			break;
	}
}

bool SolverOptions::isHelp() const
{
	return m_doHelp;
}

bool SolverOptions::isVersion() const
{
	return m_doVersion;
}

bool SolverOptions::isCopyMode() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_doCopyMode;
}

bool SolverOptions::isRange() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_doLevelSpec;
}

bool SolverOptions::isOutputFile() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_doOutputFile;
}

bool SolverOptions::isAStar() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_doAStar;
}

SolverOptions::sourceMode SolverOptions::getSourceMode() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_sourceMode;
}

unsigned int SolverOptions::getSolutionFlags() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	return m_solutionFlags;
}

const std::string& SolverOptions::getInputFilename() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	assert(m_sourceMode == INPUT_FILE);
	return m_inputFilename;
}

const std::string& SolverOptions::getOutputFilename() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	assert(m_doOutputFile);
	return m_outputFilename;
}

const RangePred& SolverOptions::getLevelSpec() const
{
	assert(!m_doHelp);
	assert(!m_doVersion);
	assert(m_doLevelSpec);
	return m_levelSpec;
}

unsigned int SolverOptions::getNumThreads() const 
{
	return m_numThreads;
}

void SolverOptions::printUsage(std::ostream& os, const char* pname) const
{
	os << "Usage: " << pname << " [-h|-v| [-a|-m|-p|-b] [-o outfile] [-r] [[-c] [infile] | -d]]\n";
	os << "If no input file is specified, the program reads from standard input. Input\n";
	os << "must be in \".mzm\" format.\n";
	
}

void SolverOptions::printExamples(std::ostream& os, const char* pname) const
{
	os << "Example: (Copy source.mzm to target.mzm, adding solutions for the specified levels.)\n";
	os << "$ " << pname << " -l -3,6,9-11,18,24- -c -o target.mzm source.mzm\n";
}
