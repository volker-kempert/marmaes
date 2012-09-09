/*
   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>

   This file is part of MARMEAS application

   MARMEAS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of
   the License, or (at your option) any later version.

   MARMEAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <fstream>
#include <stdexcept>


#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;

int main (int argc, char* argv[])
{
	   std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

	   // Create the event manager and test controller
	   CppUnit::TestResult controller;

	   // Add a listener that collects test result
	   CppUnit::TestResultCollector result;
	   controller.addListener( &result );

	   // Add the top suite to the test runner
	   CppUnit::TestRunner runner;
	   runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
	   try   {
	     std::cout << "Running "  <<  testPath;
	     runner.run( controller, testPath );

	     cerr << endl;

	     // Print test in a compiler compatible format.
	     CppUnit::CompilerOutputter outputter( &result, std::cerr );
	     outputter.write();

	     // create the cpptestresult.xml in addition
		 ofstream xmlFileOut("cpptestresults.xml");
		 CppUnit::XmlOutputter xmlOut(&result, xmlFileOut);
		 xmlOut.write();

	   }  catch ( invalid_argument& e )  {
	     std::cerr  <<  std::endl
	                <<  "ERROR: "  <<  e.what()
	                << std::endl;
	     return 1;
	   }

}

