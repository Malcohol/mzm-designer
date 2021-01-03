/* ***************************************************************************
 * solve.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <iostream>
#include <fstream>

#include <mzm/MzmReaders/DummyOStream.hpp>
#include <mzm/MzmReaders/FileMzmReader.hpp>
#include <mzm/MzmReaders/SelectiveMzmReader.hpp>
#include <mzm/RangePred/RangePred.hpp>
#include <mzm/MazezamSolver/OfflineSolver.hpp>
#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include <mzm/MazezamSolver/OutstreamSolutionCollector.hpp>

#include "SolverOptions.hpp"

using namespace mzm;
using namespace mzmslv;

/*!
 * The program name.
 */
const char* programName = "mzmsolve";
/*!
 * The current version.
 */
const char* version = "1.1";
/*!
 * Copyright information.
 */
const char* copyright = "(C) 2008 Malcolm Tyrrell";

/*!
 * Print out a version message.
 * \param pname the first argument on the command line.
 */
void versionMessage(const char* pname) 
{
	std::cout << programName << " version " << version << " " << copyright << "\n";
}

/*!
 * Print out a help message.
 * \param pname the first argument on the command line.
 */
void helpMessage(const SolverOptions& opt, const char* pname)
{
	std::cout << programName << " version " << version << " - a program which solves MazezaM levels.\n";
	std::cout << copyright << ". " << programName << " comes with NO WARRANTY.\n";
	opt.printUsage(std::cout, programName);
	opt.printFlags(std::cout, programName);
	opt.printExamples(std::cout, programName);
}

void processLevels (const SolverOptions& opt, MzmReader& r, std::ostream& os)
{
	OutstreamSolutionCollector collector(os, opt.getSolutionFlags(), opt.isCopyMode());
	WorkerPool workerPool(opt.getNumThreads() - 1);
	OfflineSolver solver(r, workerPool, collector, opt.getSolutionFlags(), opt.isCopyMode());
	solver.solve();
}

/*!
 * Determine whether the user has specified a range and do processing.
 * \param opt the options object.
 * \param r the source of the levels.
 * \param os the output stream.
 */
void handleRangeSelection (const SolverOptions& opt, MzmReader& r, std::ostream& os)
{
	if (opt.isRange()) { 
		SelectiveMzmReader sr(r, opt.getLevelSpec());
		processLevels(opt, sr, os);
	} else
		processLevels(opt, r, os);
	 
} 

/*!
 * Process levels from std::cin.
 * \param opt the options object.
 * \param os the output stream.
 */
void handleInputStandard (const SolverOptions& opt, std::ostream& os)
{
	DummyOStream dummy;
	std::ostream* copystream = &dummy;
	if (opt.isCopyMode())
		copystream = &os;
	FileMzmReader r(std::cin, "", *copystream);
	handleRangeSelection(opt,r,os);
}

/*!
 * Process levels from an input file.
 * \param opt the options object.
 * \param os the output stream.
 * \param filename the input filename.
 */
void handleInputFilename (const SolverOptions& opt, std::ostream& os, const std::string& filename)
{
	std::ifstream instream(filename.c_str());
	if (instream.fail()) {
		std::cerr << "Cannot open input file " << filename << "\n";
	} else {
		DummyOStream dummy;
		std::ostream* copystream = &dummy;
		if (opt.isCopyMode())
			copystream = &os;
		FileMzmReader r(instream, filename, *copystream);
		handleRangeSelection(opt,r,os);
	}
}

/*!
 * Determine the source of the levels and process them.
 * \param opt the options object.
 * \param os the output stream.
 */
void handleInputSelection (const SolverOptions& opt, std::ostream& os)
{
	try {
		if (opt.getSourceMode() == SolverOptions::STDIN)
			handleInputStandard(opt, os);
		else
			handleInputFilename(opt, os, opt.getInputFilename());
	} catch (FileMzmReader::FileParseError e) {
		std::cerr << e;
	}
}

/*!
 * Open the output file and process.
 * \param opt the options object.
 * \param filename the output filename.
 */
void handleOutputFilename (const SolverOptions& opt, const std::string& filename)
{
	std::ofstream outstream(filename.c_str());
	if (outstream.fail()) {
		std::cerr << "Cannot open output file " << filename << "\n";
	} else
		handleInputSelection(opt,outstream);
}

/*!
 * Determine whether the user has specified an output file and process.
 * \param opt the options object.
 */
void handleOutputSelection (const SolverOptions& opt) 
{
	if (opt.isOutputFile()) {
		handleOutputFilename(opt,opt.getOutputFilename());	
	} else
		handleInputSelection(opt,std::cout);
}

int main (int argc, char* argv[]) 
{
	try {
		SolverOptions opt;
		opt.parseOptions(argc,argv);
		if (opt.isHelp())
			helpMessage(opt, argv[0]);
		else if (opt.isVersion())
			versionMessage(argv[0]);
		else
			handleOutputSelection(opt);
		return 0;
	} catch (Options::HandledBadArgument e) {
		// The option parser prints out its own error messages, so do nothing.
		return 1;
	}
}
