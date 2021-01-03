/* ***************************************************************************
 * main.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include<iostream>
#include<fstream>

#include <mzm/MzmReaders/FileMzmReader.hpp>
#include <mzm/MzmReaders/DummyOStream.hpp>

#include "CursesInit.hpp"
#include "Designer.hpp"
#include "DesignerOptions.hpp"

using namespace mzm;

/*!
 * The program name.
 */
static const char* programName = "mzm-designer";
/*!
 * The current version.
 */
static const char* version = "1.0";
/*!
 * Copyright information.
 */
static const char* copyright = "(C) 2009 Malcolm Tyrrell";

/*!
 * Print out a version message.
 * \param pname the first argument on the command line.
 */
static void versionMessage(const char* pname) 
{
	std::cout << programName << " version " << version << " " << copyright << "\n";
}

/*!
 * Print out a help message.
 * \param pname the first argument on the command line.
 */
static void helpMessage(const DesignerOptions& opt, const char* pname)
{
	std::cout << programName << " version " << version << " - an editor for MazezaM levels.\n";
	std::cout << copyright << ". " << programName << " comes with NO WARRANTY.\n";
	opt.printUsage(std::cout, programName);
	opt.printFlags(std::cout, programName);
	opt.printExamples(std::cout, programName);
}

/*!
 * Set up curses and launch the designer.
 * \param opt the options object.
 * \param outstream the stream to which levels should be saved.
 * \param startLevel the level number for saving.
 */
static void design(const DesignerOptions& opt, std::ostream& outstream, MazezamData *startLevel, int levelNum)
{
	if (startLevel) {
		/*
		MazezamSolverJobFewestPushes job(*startLevel);
		job.doJob();
		mzm_path path;
		job.addToMzmPath(path);
		std::cout << path << "\n";
		*/
		Designer d(!opt.isMonochrome(), opt.getNumThreads() - 1, outstream, *startLevel, levelNum );
		delete startLevel;
		d.designLevel();
	} else {
		Designer d(!opt.isMonochrome(), opt.getNumThreads() - 1, outstream, levelNum );
		d.designLevel();
	}
}

static void findLevelAndCountLevels(std::istream& is, const std::string& fileName, int levelNum, MazezamData*& level, int& numOfLevels)
{
	// We have to find the number of levels and the start level from the outstream.
	level = NULL;
	numOfLevels = 0;
	{	
		DummyOStream dos;
		FileMzmReader reader(is, fileName, dos);
		while (reader.next()) {
			if (levelNum == numOfLevels + 1)
				level = reader.getLevel();
			else if (levelNum == -1) {
				if (level)
					delete level; 
				level = reader.getLevel();
			}
			++numOfLevels;
		}
	}
}

/*!
 * Open the output file and process.
 * \param opt the options object.
 */
static int handleFiles(const DesignerOptions& opt)
{
	// Find the number of levels in the output file so we can set an appropriate save number.
	std::fstream outstream(opt.getOutputFilename().c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::app);
	if (outstream.fail()) {
		std::cerr << "Cannot open output file " << opt.getOutputFilename() << std::endl;
		return 1;
	}
	MazezamData* startLevel = NULL;
	int levelNum;	
	try {
		if (opt.getInputFilename() != "") {
			// Parse the input file to find the start level.
			std::fstream instream(opt.getInputFilename().c_str(), std::ios_base::in);
			if (instream.fail()) {
				std::cerr << "Cannot open input file " << opt.getInputFilename() << std::endl;
				return 1;
			}
			int dummyLevelNum;
			findLevelAndCountLevels(instream, opt.getInputFilename(), opt.getLevelNumber(), startLevel, dummyLevelNum);
			// Parse the output file to find the number of levels.
			MazezamData* dummyLevel;
			findLevelAndCountLevels(outstream, opt.getOutputFilename(), -2, dummyLevel, levelNum);
		} else {
			// Parse the output file to find both the start level and the number of levels.
			findLevelAndCountLevels(outstream, opt.getOutputFilename(), opt.getLevelNumber(), startLevel, levelNum);
		}
	} catch (FileMzmReader::FileParseError e) {
		std::cerr << e << std::endl;
		return 1;
	}
	// We've finished using the outstream for input.
	assert(outstream.eof());
	// Clearing the flags so we can use it for output.
	outstream.clear();
	if (startLevel == NULL ) {
		if ( opt.getLevelNumber() != -1) {
			std::cerr << "Cannot find level " << opt.getLevelNumber() << " in file " << opt.getInputFilename() << std::endl;
			return 1;
		} else
			levelNum = 1;
	}
	design(opt, outstream, startLevel, levelNum);
	return 0;
}
extern int solve(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	//solve(argc, argv);
	try {
		DesignerOptions opt;
		opt.parseOptions(argc,argv);
		if (opt.isHelp())
			helpMessage(opt, argv[0]);
		else if (opt.isVersion())
			versionMessage(argv[0]);
		else {
			return handleFiles(opt);
		}
	} catch (Options::HandledBadArgument e) {
		// The option parser prints out its own error messages, so do nothing.
		return 1;
	}
}
