#include <filesystem>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>

#include <parser/Instance.h>

using namespace parser::pddl;

template < typename T >
void checkEqual( T & prob, const std::string & file ) {
	std::ifstream f(file.c_str());
	if (!f) throw std::runtime_error(std::string("Failed to open file '") + file + "'");
	std::string s, t;
	
	while(std::getline(f, s)){
		t += s + "\n";
	}

	std::ostringstream ds;
	ds << prob;

	std::string output = ds.str();

	std::ofstream of(std::filesystem::path("./output") / std::filesystem::path(file).filename());
	of << output;

	ASSERT_EQ( t, output );
}

TEST(DomainTests, LogisticsTest)
{
	std::cout << std::filesystem::current_path() << std::endl;

	Domain dom( "domains/Log_dom.pddl" );
	Instance ins( dom, "domains/Log_ins.pddl" );

	checkEqual( dom, "expected/Log_dom.pddl" );
	checkEqual( ins, "expected/Log_ins.pddl" );
}

TEST(DomainTests, ScheduleTest)
{
	Domain dom( "domains/Sched_dom.pddl" );
	Instance ins( dom, "domains/Sched_ins.pddl" );

	checkEqual( dom, "expected/Sched_dom.pddl" );
	checkEqual( ins, "expected/Sched_ins.pddl" );
}

TEST(DomainTests, ElevatorTest)
{
	Domain dom( "domains/Elev_dom.pddl" );
	Instance ins( dom, "domains/Elev_ins.pddl" );

	checkEqual( dom, "expected/Elev_dom.pddl" );
	checkEqual( ins, "expected/Elev_ins.pddl" );
}

TEST(DomainTests, TemporalTest)
{
	Domain dom( "domains/Mapana_dom.pddl" );
	Instance ins( dom, "domains/Mapana_ins.pddl" );

	checkEqual( dom, "expected/Mapana_dom.pddl" );
	checkEqual( ins, "expected/Mapana_ins.pddl" );
}

int main(int argc, char **argv) 
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/*
class DomainTests : public testing::Test
{
public:
	TEST_FIXTURE( DomainTests )
	{
		TEST_CASE( logisticsTest );
		TEST_CASE( scheduleTest );
		TEST_CASE( elevatorTest );
		TEST_CASE( temporalTest );
	}

	template < typename T >
	void checkEqual( T & prob, const std::string & file ) {
		std::ifstream f(file.c_str());
		if (!f) throw std::runtime_error(std::string("Failed to open file '") + file + "'");
		std::string s, t;
		
		while(std::getline(f, s)){
			t += s + "\n";
		}

		std::ostringstream ds;
		ds << prob;
		ASSERT_EQUALS( t, ds.str() );
		std::ofstream of("ins.txt");
		of<<ds.str();
	}

	void logisticsTest() {
		Domain dom( "domains/Log_dom.pddl" );
		Instance ins( dom, "domains/Log_ins.pddl" );

		checkEqual( dom, "tests/expected/Log_dom.pddl" );
		checkEqual( ins, "tests/expected/Log_ins.pddl" );
	}

	void scheduleTest() {
		Domain dom( "domains/Sched_dom.pddl" );
		Instance ins( dom, "domains/Sched_ins.pddl" );

		checkEqual( dom, "tests/expected/Sched_dom.pddl" );
		checkEqual( ins, "tests/expected/Sched_ins.pddl" );
	}

	void elevatorTest() {
		Domain dom( "domains/Elev_dom.pddl" );
		Instance ins( dom, "domains/Elev_ins.pddl" );

		checkEqual( dom, "tests/expected/Elev_dom.pddl" );
		checkEqual( ins, "tests/expected/Elev_ins.pddl" );
	}

	void temporalTest() {
		Domain dom( "domains/Mapana_dom.pddl" );
		Instance ins( dom, "domains/Mapana_ins.pddl" );

		checkEqual( dom, "tests/expected/Mapana_dom.pddl" );
		checkEqual( ins, "tests/expected/Mapana_ins.pddl" );
	}
};*/

//TEST(DomainTests, )

//REGISTER_FIXTURE( DomainTests )

