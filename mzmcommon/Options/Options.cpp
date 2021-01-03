/* ***************************************************************************
 * Options.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "Options.hpp"
#include <getopt.h>
#include <cassert>
#include <ostream>
#include <iostream>
#include <cstring>
#include <string>


void Options::errorIf(bool b, const char* message, const char* pname) const
{
	if (b) {
		std::cerr << pname << ": " << message << "\n";
		throw Options::HandledBadArgument();
	}
}


void Options::errorIf(bool b, const char* message, const char* opt, const char* pname) const 
{
	if (b) {
		std::cerr << pname << ": " << opt << ": " << message << "\n";
		throw Options::HandledBadArgument();
	}
}


void Options::buildShortOptions(std::string& shortOptions) const
{ 
	for(int i = 0; i < m_numOfOptionsInfo; ++i) {
		shortOptions += m_optionsInfo[i].val;
		if (m_optionsInfo[i].has_arg != no_argument)
			shortOptions += ':';
		if (m_optionsInfo[i].has_arg == optional_argument)
			shortOptions += ':';
	}
}


void Options::buildLongOptions(std::vector<option>& longOptions) const
{
	for(int i = 0; i < m_numOfOptionsInfo; ++i) {
		option newOption;
		newOption.name = m_optionsInfo[i].name;
		newOption.has_arg = m_optionsInfo[i].has_arg;
		newOption.flag = m_optionsInfo[i].flag;
		newOption.val = m_optionsInfo[i].val;
		longOptions.push_back(newOption);
	}
	option lastOption;
	// Terminate the array with a row of zeroes.
	lastOption.name = 0;
	lastOption.has_arg = 0;
	lastOption.flag = 0;
	lastOption.val = 0;
	longOptions.push_back(lastOption);
}

Options::Options(const OptionsInfo* optionsInfo, int numOfOptionsInfo)
:	m_optionsInfo(optionsInfo)
,	m_numOfOptionsInfo(numOfOptionsInfo)
{
}

void Options::parseOptions(int argc, char* argv[])
{
	// Use getopt's own error messages when applicable.
	opterr = 1;
	int c;
	
	// Create short options.
	std::string short_options;
	buildShortOptions(short_options);

#ifdef _GNU_SOURCE
	// Create long options.
	std::vector<option> long_options;
	buildLongOptions(long_options);

	int option_index;
	while ((c = getopt_long(argc,argv,short_options.c_str(),&long_options.front(),&option_index)) != -1) {
#else
	// Compiling with an ordinary POSIX getopt.
	while ((c = getopt(argc,argv,*short_options.c_str()))!= -1) {
#endif
		handleOption(c,optarg,argv[0],argv[optind - 2]);
	}
}


Options::~Options()
{
}


unsigned int Options::lineSize(int i) const
{
	assert(0 <= i);
	assert(i < m_numOfOptionsInfo);
#ifdef _GNU_SOURCE
	// Short and long options.
	if (m_optionsInfo[i].has_arg == no_argument)
		return 7 + strlen(m_optionsInfo[i].name);
	else if (m_optionsInfo[i].has_arg == required_argument)
		return 9 + strlen(m_optionsInfo[i].name) + strlen(m_optionsInfo[i].argumentName) * 2;
	else
		return 13 + strlen(m_optionsInfo[i].name) + strlen(m_optionsInfo[i].argumentName) * 2;
#else
	// Just short options
	if (m_optionsInfo[i].has_arg == no_argument)
		return 3;
	else if (m_optionsInfo[i].has_arg == required_argument)
		return 4 + strlen(m_optionsInfo[i].argumentName);
	else
		return 6 + strlen(m_optionsInfo[i].argumentName);
#endif
}


void Options::printFlags(std::ostream& os, const char* pname) const
{
	os << "Options:\n";
	// find column position for descriptions.
	unsigned int descpos = 0;
	for (int i = 0; i < m_numOfOptionsInfo; ++i)
		descpos = std::max(descpos,lineSize(i));
	// output flag table.
	for (int i = 0; i < m_numOfOptionsInfo; ++i) {
		os << " -" << char(m_optionsInfo[i].val);
		if (m_optionsInfo[i].has_arg == required_argument)
			os << " " << m_optionsInfo[i].argumentName;
		else if (m_optionsInfo[i].has_arg == optional_argument)
			os << " [" << m_optionsInfo[i].argumentName << "]";
#ifdef _GNU_SOURCE
		// Also long options.
		os << ", --" << m_optionsInfo[i].name;
		if (m_optionsInfo[i].has_arg == required_argument)
			os << "=" << m_optionsInfo[i].argumentName;
		else if (m_optionsInfo[i].has_arg == optional_argument)
			os << "[=" << m_optionsInfo[i].argumentName << "]";
#endif
		os << std::string(descpos - lineSize(i) + 2,' ') << m_optionsInfo[i].description << "\n";
	}
}
