//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _main_h
#define _main_h

#include <utility> //For pair
#include <map>
#include <iostream>

#include <cctype> //for tolower

#include <physgame.h> // For String and all of physgame


// Define some Results of tests
enum TestResult
{
    Success         = 0,
    Skipped         = 1,
    Cancelled        = 2,        // Was canceled by user, so success is unknown, but user doesn't cared
    Inconclusive    = 3,        // if a user answers that they don't know what happened in a test that involved interaction, it likely worked, but we can't be sure
    Failed          = 4,        // Known failure
    Unknown         = 5         // Since we don't know what happed this is the worst kind of failure
};

phys::String TestResultToString(TestResult Convertable)
{
    switch(Convertable)
    {
        case Success:
            return "Success";
        case Skipped:
            return "Skipped";
        case Cancelled:
            return "Cancelled";
        case Inconclusive:
            return "Inconclusive";
        case Failed:
            return "Failed";
        case Unknown:
            return "Unknown";
        default:
            throw(phys::Exception(phys::StringCat("Cannot convert TestResult with value ", phys::ToString(Convertable))));
    }
}

phys::String MakePadding(phys::String Leader, unsigned int Column)
{
    phys::String Spaces(" ");
    for (unsigned int c=Leader.length(); c<Column;++c)
        { Spaces+=" "; }
    return Spaces;
}

// the return type of tests
typedef std::pair<phys::String,TestResult> TestData;

// The classes for Tests themselves
// inherits from std::map to make storage location of of the TestData obvious
typedef std::map<phys::String,TestResult> TestDataStorage;
class UnitTest : public TestDataStorage
{
    protected:

        // Some basic variable for tracking simple statistics
        TestData MostSuccessful;
        TestData LeastSuccessful;
        unsigned int LongestNameLength;

    public:
        UnitTest() :
            MostSuccessful("",Unknown),
            LeastSuccessful("",Success),
            LongestNameLength(0)
        {}

        // This is expected to run all the tests that meet the criteria passed in
        // This should return the LeastSuccessful TestResult, this will make it easier for the main to find and report errors
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests) = 0;

        // It is expected that tests will be inserted using these, this will automate tracking of the most and least successful tests
        void AddTestResult(TestData FreshMeat)
        {
            if(FreshMeat.second<MostSuccessful.second)
                { MostSuccessful=FreshMeat; }
            if(FreshMeat.second>LeastSuccessful.second)
                { LeastSuccessful=FreshMeat; }

            if(FreshMeat.first.length()>LongestNameLength)
                { LongestNameLength=FreshMeat.first.length(); }

            this->insert(FreshMeat);
        }

        // It is expected that every member of a class in physgame will be tested and its full name, include scoping operators, namespace,
        // class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
        // and arguments if required as the testname.
        // Example TestNames (The Fresh parameter)
        //      "phys::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
        //      "phys::Vector2::operator+"              //Test of only operator+ on Vector2
        //      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
        void AddTestResult(const phys::String Fresh, TestResult Meat)
            { AddTestResult(TestData(Fresh,Meat)); }

        // make it a little easier to aggregate answers in one place
        const UnitTest& operator+=(const UnitTest& rhs)
        {
            if(rhs.MostSuccessful.second<MostSuccessful.second)
                { MostSuccessful=rhs.MostSuccessful; }
            if(rhs.MostSuccessful.second>LeastSuccessful.second)
                { LeastSuccessful=rhs.MostSuccessful; }

            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
        }

        void DisplayResults(bool Summary = true, bool FullOutput = true)
        {
            vector<unsigned int> TestCounts;
            TestCounts.insert(TestCounts.end(),1+(unsigned int)Unknown, 0);

            phys::String TestName("Test Name");
            cout << endl << " " << TestName << MakePadding(TestName, LongestNameLength) << "Result" << endl;
            for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
            {
                if(FullOutput)
                {
                    cout << Iter->first << MakePadding(Iter->first, LongestNameLength+1) << TestResultToString(Iter->second) << endl;
                }
                TestCounts.at((unsigned int)Iter->second)++;
            }

            if(Summary)
            {
                cout << endl << " Results Summary:" << endl;
                for(unsigned int c=0; c<TestCounts.size();++c)
                {
                    phys::String ResultName(TestResultToString((TestResult)c));
                    cout << "  " << ResultName << MakePadding(ResultName,16) << TestCounts.at(c) << endl;
                }
                cout << endl;
            }
        }

};

// Drops a String to all lower case, changes the string passed in
char* AllLower(char* CString)
{
    locale loc;
    for(int c=0; CString[c]!='\0'; ++c)
    {
        CString[c]=tolower(CString[c],loc);
    }
    return CString;
}

// Some simple functions for formatting user input/output
phys::String BoolToString(bool i)
    { return i?"True":"False" ; }

// Picks up on
//      True, Yes as Success
//      False, No as Failed
//      Cancel as Canceled
//      Unsure, Inconclusive as Inconclusive
TestResult GetTestAnswer(phys::String Question)
{
    phys::String Input;
    char Answer;

    while(true)
    {
        cout << Question;
        getline(cin, Input);
        stringstream InputStream(Input);
        if (InputStream >> Answer)
        {
            Answer=tolower(Answer);
            if (Answer=='t' || Answer=='y' || Answer=='f' || Answer=='n' || Answer=='c' || Answer=='u' || Answer=='i')
                { break; }
        }

        cout << endl << "Expected (T)rue/(Y)es for Success, (F)alse/(N)o for Failure," << endl << " (C)anceled to cancel this test, or (U)nsure/(I)nconclusive if you don't know." << endl << endl;
    }

    switch(Answer)
    {
        case 't': case 'y':
            return Success;
        case 'f': case 'n':
            return Failed;
        case 'c':
            return Cancelled;
        case 'u': case 'i':
            return Inconclusive;
        default:
            return Unknown;
    }

}

///Possible ways to exit the UnitTest Program
enum ExitCodes
{
    ExitSuccess             = 0,
    ExitInvalidArguments    = 1
};

int Usage(phys::String ThisName)
{
    std::cout   << endl << "Usage: " << ThisName << " [summary] [[interactive][automatic]] [all] Test Group Names ..." << endl << endl
                << "All:         All tests in all groups will be run." << endl
                << "Interactive: Only interactive tests will be performed on specified test groups." << endl
                << "Automatic:   Only automated tests will be performed on specified test groups." << endl
                << "Interactive and Automatic: All tests will be run on specificied test groups." << endl << endl
                << "Summary:     Only display a count of failures and successes" << endl << endl
                << "If only test group names are entered, then all tests in those groups are run." << endl
                << "This command is not case sensitive." << endl << endl
                << "Current Test Groups: " << endl
                << "\tCompilerFlag \tVector2" << endl << endl;//<< \tVector2" <<endl;
    return ExitInvalidArguments;
}






#endif