//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#include <vector>
#include <iostream>
#include <fstream>

#include <cctype> // For tolower
#include <cstdio> // For remove

#include <mezzanine.h> // For String and all of Mezzanine


// Define some Results of tests
enum TestResult
{
    Success         = 0,        // test was ran and appeared to work
    Skipped         = 1,        // Test was simply not ran
    Cancelled       = 2,        // Was canceled by user, so success is unknown, but user doesn't cared
    Inconclusive    = 3,        // if a user answers that they don't know what happened in a test that involved interaction, it likely worked, but we can't be sure
    Failed          = 4,        // Known failure
    Unknown         = 5,        // Since we don't know what happened this is the worst kind of failure
    NotApplicable   = 6         // This is not even a kind of failure, This is used to when referencing a test, so if this winds up coming out of a test, then something has failed
};

// convert the above enum to a Striong that matchins the in-code name.
Mezzanine::String TestResultToString(TestResult Convertable)
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
        case NotApplicable:
            return "N/A";
        default:
            { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Cannot convert to String from TestResult " + Mezzanine::ToString(Convertable)); }
    }
}

TestResult StringToTestResult(Mezzanine::String Text)
{
    if(Text.size()==0)
        { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Cannot convert to TestResult from empty String"); }

    switch(Text.at(0))
    {
        case 'S':
            if ( "Success" == Text )
                return Success;
            return Skipped;
        case 'C':
            return Cancelled;
        case 'I':
            return Inconclusive;
        case 'U':
            return Unknown;
        case 'F':
            return Failed;
        case 'N':
            return NotApplicable;
        default:
            { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text " + Text); }
    }
    return Unknown;
}

Mezzanine::String rtrim(const Mezzanine::String &t)
{
    Mezzanine::String str = t;
    size_t found;
    found = str.find_last_not_of(" \n\r\t");
    if (found != Mezzanine::String::npos)
        { str.erase(found+1); }
    else
        { str.clear(); }            // str is all whitespace

    return str;
}

// Used for padding spaces, after a piece of leader text, such that it always ends at teh expected colum
Mezzanine::String MakePadding(Mezzanine::String Leader, unsigned int Column)
{
    Mezzanine::String Spaces(" ");
    for (unsigned int c=Leader.length(); c<Column;++c)
        { Spaces+=" "; }
    return Spaces;
}

// the return type of tests
typedef std::pair<Mezzanine::String,TestResult> TestData;

TestData StringToTestData(Mezzanine::String Line)
{
    TestData Results;
    size_t LastSpace=Line.rfind(' ');
    Results.second=StringToTestResult(Line.substr(LastSpace+1,100)); // No testdata should be longer than 100
    Results.first=rtrim(Line.substr(0,LastSpace));
    return Results;
}

// The classes for Tests themselves
// inherits from std::map to make storage location of of the TestData obvious
typedef std::map<Mezzanine::String,TestResult> TestDataStorage;

class UnitTestGroup : public TestDataStorage
{
    protected:
        // Some basic variable for tracking simple statistics
        unsigned int LongestNameLength;

    public:
        UnitTestGroup() :
            LongestNameLength(0)
        {}

// Simply naming tests
// make iterating over tests that could be possible
// use to set skips and run

        // This is expected to run all the tests that meet the criteria passed in
        // This should return the LeastSuccessful TestResult, this will make it easier for the main to find and report errors
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
            {}

        // This is used to control the behavior of the function AddTestResult(), this can optionally be passed as a third argument
        // to prioritize whether newer vs older or0 successful vs failing results are more important
        enum OverWriteResults{
            OverWriteIfLessSuccessful,
            OverWriteIfMoreSuccessful,
            OverWrite,
            DoNotOverWrite
        };

        // It is expected that tests will be inserted using these, this will automate tracking of the most and least successful tests
        void AddTestResult(TestData FreshMeat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
        {
            bool Added=false;

            TestDataStorage::iterator PreExisting = this->find(FreshMeat.first);
            if(this->end()!=PreExisting)
            {
                switch(Behavior)
                {
                    case OverWriteIfLessSuccessful:
                        if (PreExisting->second <= FreshMeat.second)
                            { PreExisting->second = FreshMeat.second; Added=true; }
                        break;
                    case OverWrite:
                        PreExisting->second = FreshMeat.second;
                        Added=true;
                        break;
                    case OverWriteIfMoreSuccessful:
                        if (PreExisting->second >= FreshMeat.second)
                            { PreExisting->second = FreshMeat.second; Added=true; }
                        break;
                    case DoNotOverWrite:
                        break;
                }
            }else{
                this->insert(FreshMeat);
                Added=true;
            }

            if (Added)
            {
                if(FreshMeat.first.length()>LongestNameLength)
                    { LongestNameLength=FreshMeat.first.length(); }
            }
        }

        // It is expected that every member of a class in Mezzanine will be tested and its full name, include scoping operators, namespace,
        // class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
        // and arguments if required as the testname.
        // Example TestNames (The Fresh parameter)
        //      "Mezzanine::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
        //      "Mezzanine::Vector2::operator+"              //Test of only operator+ on Vector2
        //      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
        void AddTestResult(const Mezzanine::String Fresh, TestResult Meat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
            { AddTestResult(TestData(Fresh,Meat),Behavior); }

        // make it a little easier to aggregate answers in one place
        const UnitTestGroup& operator+=(const UnitTestGroup& rhs)
        {
            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
        }

        virtual void DisplayResults(std::ostream& Output=std::cout, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true)
        {
            std::vector<unsigned int> TestCounts; // This will store the counts of the Sucesses, failures, etc...
            TestCounts.insert(TestCounts.end(),1+(unsigned int)Unknown, 0); //Fill with the exact amount of 0s

            if(FullOutput && HeaderOutput) // No point in displaying the header without the other content.
            {
                Mezzanine::String TestName("Test Name");
                Output << std::endl << " " << TestName << MakePadding(TestName, LongestNameLength) << "Result" << std::endl;
            }

            for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
            {
                if(FullOutput)
                {
                    Output << Iter->first << MakePadding(Iter->first, LongestNameLength+1) << TestResultToString(Iter->second) << std::endl;
                }
                TestCounts.at((unsigned int)Iter->second)++; // Count this test result
            }

            if(Summary)
            {
                Output << std::endl << " Results Summary:" << std::endl;
                for(unsigned int c=0; c<TestCounts.size();++c)
                {
                    Mezzanine::String ResultName(TestResultToString((TestResult)c));
                    Output << "  " << ResultName << MakePadding(ResultName,16) << TestCounts.at(c) << std::endl;
                }
                Output << std::endl;
            }
        }


        virtual bool AddSuccessFromBool(Mezzanine::String TestName, bool Condition)
        {
            if(Condition)
            {
                AddTestResult(TestName, Success, UnitTestGroup::OverWrite);
            }else{
                AddTestResult(TestName, Failed, UnitTestGroup::OverWrite);
            }
            return Condition;
        }
};

// The list of all the testgroups
std::map<Mezzanine::String, UnitTestGroup*> TestGroups;

// Drops a String to all lower case, changes the string passed in
char* AllLower(char* CString)
{
    std::locale loc;
    for(int c=0; CString[c]!='\0'; ++c)
    {
        CString[c]=tolower(CString[c],loc);
    }
    return CString;
}

// Some simple functions for formatting user input/output
Mezzanine::String BoolToString(bool i)
    { return i?"True":"False" ; }

// Picks up on
//      True, Yes as Success
//      False, No as Failed
//      Cancel as Canceled
//      Unsure, Inconclusive as Inconclusive
TestResult GetTestAnswer(Mezzanine::String Question)
{
    Mezzanine::String Input;
    char Answer;

    while(true)
    {
        std::cout << Question;
        getline(std::cin, Input);
        std::stringstream InputStream(Input);
        if (InputStream >> Answer)
        {
            Answer=tolower(Answer);
            if (Answer=='t' || Answer=='y' || Answer=='f' || Answer=='n' || Answer=='c' || Answer=='u' || Answer=='i')
                { break; }
        }

        std::cout << std::endl << "Expected (T)rue/(Y)es for Success, (F)alse/(N)o for Failure," << std::endl << " (C)anceled to cancel this test, or (U)nsure/(I)nconclusive if you don't know." << std::endl << std::endl;
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

///Possible ways to exit the UnitTestGroup Program
enum ExitCodes
{
    ExitSuccess             = 0,
    ExitInvalidArguments    = 1
};

int Usage(Mezzanine::String ThisName)
{
    std::cout   << std::endl << "Usage: " << ThisName << " [help] [summary] [testlist] [interactive|automatic] [all] Test Group Names ..." << std::endl << std::endl
                << "All:         All test groups will be run." << std::endl
                << "Interactive: Only interactive tests will be performed on specified test groups." << std::endl
                << "Automatic:   Only automated tests will be performed on specified test groups." << std::endl
                //<< "Interactive and Automatic: All tests will be run on specificied test groups." << std::endl << std::endl
                << "Summary:     Only display a count of failures and successes" << std::endl
                << "testlist:    Output a list of all tests, one per line" << std::endl
                << "Help:        Display this message"<< std::endl << std::endl
                << "If only test group names are entered, then all tests in those groups are run." << std::endl
                << "This command is not case sensitive." << std::endl << std::endl
                << "Current Test Groups: " << std::endl;
    Mezzanine::Whole c = 0;
    for(std::map<Mezzanine::String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
    {
        std::cout << "\t" << Iter->first << (Iter->first.size()<7?"\t":"") << " ";
        ++c;        //enforce 4 names per line
        if (4==c)
            { std::cout << std::endl; c=0; }
    }
    std::cout << std::endl;

    return ExitInvalidArguments;
}

int PrintList()
{
    for(std::map<Mezzanine::String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
        { std::cout << Iter->first << std::endl; }
    return ExitSuccess;
}

/////////////////////////////////////////////////////////////////////////////////////
//This next block of code creates a minimal engine environment for testing managers and other ngine components
using namespace Mezzanine;

Entresol* TheEntresol;
Mezzanine::UI::Caption *TheTextW1;
Mezzanine::UI::Caption *TheTextB1;
Mezzanine::UI::Caption *TheTextW2;
Mezzanine::UI::Caption *TheTextB2;
String TheMessage;
String TheMessage2;
SimpleTimer *ThisTimer;
TestResult AnswerToQuestion = Unknown;

void StartEngine()
{
    PhysicsConstructionInfo Info;
    Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_LimitlessWorld | PhysicsConstructionInfo::PCF_SoftRigidWorld);
    TheEntresol = new Entresol(Info,"DefaultSceneManager","plugins.cfg","data/");
    TheEntresol->EngineInit(false);
    ResourceManager::GetSingletonPtr()->AddAssetLocation("data", FileSystem, "files", false);
    GraphicsManager::GetSingletonPtr()->CreateGameWindow("EventManager Test",800,600,0);
    UIManager::GetSingletonPtr()->LoadMTA("dejavu");
    ResourceManager::GetSingletonPtr()->InitAssetGroup("files");
    Mezzanine::UI::Screen* TheScreen = UIManager::GetSingletonPtr()->CreateScreen("Screen","dejavu",GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetViewport(0));

    Mezzanine::Entresol::GetSingletonPtr()->Log("Found Resolutions:");
    const std::vector<String>* Resolutions = Mezzanine::Entresol::GetSingletonPtr()->GetGraphicsManager()->GetSupportedResolutions();
    for (Whole Count=0; Count<Resolutions->size(); ++Count)
    {
        Mezzanine::Entresol::GetSingletonPtr()->Log(Resolutions->at(Count));
    }
    Mezzanine::Entresol::GetSingletonPtr()->CommitLog();

    UI::OpenRenderableContainerWidget* HUDContainer = TheScreen->CreateOpenRenderableContainerWidget("HUD");
    TheScreen->AddRootWidget(0,HUDContainer);

    TheTextB1 = HUDContainer->CreateCaption(ConstString("TheTextB1"),UI::RenderableRect(Vector2(0.0016,0.603),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextB1->SetTextColour(ColourValue::Black());
    TheTextB1->SetBackgroundColour(ColourValue::Transparent());
    TheTextW1 = HUDContainer->CreateCaption(ConstString("TheTextW1"),UI::RenderableRect(Vector2(0,0.6),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextW1->SetTextColour(ColourValue::White());
    TheTextW1->SetBackgroundColour(ColourValue::Transparent());

    TheTextB2 = HUDContainer->CreateCaption(ConstString("TheTextB2"),UI::RenderableRect(Vector2(0.0016,0.753),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextB2->SetTextColour(ColourValue::Black());
    TheTextB2->SetBackgroundColour(ColourValue::Transparent());
    TheTextW2 = HUDContainer->CreateCaption(ConstString("TheTextW2"),UI::RenderableRect(Vector2(0,0.75),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextW2->SetTextColour(ColourValue::White());
    TheTextW2->SetBackgroundColour(ColourValue::Transparent());

}

void UpdateMessage( String Message, String Message2)
{
    TheTextB1->SetText( Message );
    TheTextW1->SetText( Message );
    TheTextB2->SetText( Message2 );
    TheTextW2->SetText( Message2 );
}

// Countdown callbacks
bool PostTimerEnd()
{
    return false;
    TimerManager::GetSingletonPtr()->SetPostMainLoopItems(0);
}

bool PostTimerUpdate()
    { UpdateMessage(TheMessage,ToString(int(ThisTimer->GetCurrentTime()/1000000))); return true; }

class TimerEnding : public TimerCallback
{
    virtual void DoCallbackItems()
        { TimerManager::GetSingletonPtr()->SetPostMainLoopItems(&PostTimerEnd); }
};
TimerEnding* CountDownCallback;

void StopEngine()
{
    delete CountDownCallback;
    delete TheEntresol;
    crossplatform::WaitMilliseconds(1000); // Ogre spawns some stuff in a seperate thread this is more then enough time for it to finish
}

bool PostInputCheck()
{
    UpdateMessage(TheMessage,TheMessage2);
    AnswerToQuestion=Unknown;
    EventManager* EventMan = EventManager::GetSingletonPtr();
    EventUserInput* ThisInput = EventMan->PopNextUserInputEvent();
    while (ThisInput) //for each userinput event
    {
        for (Whole c=0; c<ThisInput->GetMetaCodeCount(); ++c ) //for each metacode in each userinput
        {
            Input::InputCode ThisCode = ThisInput->GetMetaCode(c).GetCode();
            switch(ThisCode)
            {
                case Input::KEY_T: case Input::KEY_Y: // True or Yes
                    AnswerToQuestion = Success;
                    break;
                case Input::KEY_F: case Input::KEY_N: // False or No
                    AnswerToQuestion = Failed;
                    break;
                case Input::KEY_C:                       // Cancel
                    AnswerToQuestion = Cancelled;
                    break;
                case Input::KEY_U: case Input::KEY_I: // Unknown or Inconclusive
                    AnswerToQuestion = Inconclusive;
                    break;
                default:
                    AnswerToQuestion = Unknown;
                    break;
            }
        }
        delete ThisInput;
        ThisInput = EventMan->PopNextUserInputEvent();
    }

    if (Unknown==AnswerToQuestion)
        { return true; }
    else
        { return false; }
}

// Use this to start a countdown in the interactive test
void StartCountdown(Whole Seconds)
{
    ThisTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::StopWatch);
    ThisTimer->SetInitialTime(Seconds * 1000000);
    ThisTimer->Reset();
    ThisTimer->SetGoalTime(0);
    ThisTimer->Start();
    CountDownCallback = new TimerEnding;
    ThisTimer->SetCallback(CountDownCallback);
    TimerManager::GetSingletonPtr()->SetPostMainLoopItems(&PostTimerUpdate);
}

void GetAnswer()
{
    TheMessage2 = "T/Y:Yes  F/N:No  C:Cancel  U/I:Inconclusive";
    UpdateMessage(TheMessage, TheMessage2);
    EventManager::GetSingletonPtr()->SetPostMainLoopItems(&PostInputCheck);
}

///////////////////////////////////////////////////////////////////////////////
// Temp File Manipulation
///////////////////////////////////////

/// @internal
/// @brief This is the name of the file used to communicate results from child processes
/// @warning This variable is used to create temporary files in a percieved insecure way
/// Everything will be fine as long as nothing else writes to this this file during or
/// between Tests. If something does, then you probably have big enough problems you
/// shouldn't be developing software until that is fixed.
static const String TempFile("UnitTestWork.txt");

/// @internal
/// @brief Empty the file specified by @ref TempFile
/// @warning This doesn't ask for permission and can't easily be cancelled or recovered
/// from. This will open, then erase the contents of the file.
/// @throw This can throw any exception that the C++ filestream classes can throw.
void ClearTempFile()
{
    std::ofstream FileToClear;
    FileToClear.open(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc); // Clear the work file.
    FileToClear.close();
}

/// @internal
/// @brief This will open then parse the contents of the file specified by @ref TempFile and interpret any test results located
/// @throw This can throw any exception that the C++ filestream classes can throw.
/// @return This "reads" the temp file and interprets it. It tries to extract the name of the test as the whole of a line minus
/// the last word. The last word is then converted into a @ref TestResult using @ref StringToTestResult . Any Whitespace between
/// between the end of the last word and the end of the test name is dropped. All lines are interpretted this way and returned
/// as a single @ref UnitTestGroup.
UnitTestGroup GetResultsFromTempFile()
{
    UnitTestGroup Results;
    std::vector<Mezzanine::String> FileContents;

    char SingleLine[1024];
    std::ifstream TheSourceFile(TempFile.c_str());
    while( TheSourceFile.good() )
    {
        TheSourceFile.getline(SingleLine,1024);
        FileContents.push_back(Mezzanine::String(SingleLine));
    }

    if(TheSourceFile.eof()) // We successfully reached the end of the file
        { }
    if(TheSourceFile.bad()) // We fail somehow
        { std::cerr << "Error reading temp file." << std::endl; }
    TheSourceFile.close();

    for(std::vector<Mezzanine::String>::iterator Iter=FileContents.begin(); FileContents.end()!=Iter; ++Iter) // for each line in the file, that is now loaded in RAM
    {
        if(rtrim(*Iter).size()) // If there is more than whitespace
        {
            Results.AddTestResult(StringToTestData(*Iter));
        }
    }

    return Results;
}

/// @brief Attempts to delete temp file. Silently fails if not possible.
void DeleteTempFile()
{
    remove(TempFile.c_str());
}

#endif
