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

class DomainTests : public testing::Test
{
public:

	void logisticsTest() {
		Domain dom( "domains/Log_dom.pddl" );
		Instance ins( dom, "domains/Log_ins.pddl" );

		checkEqual( dom, "expected/Log_dom.pddl" );
		checkEqual( ins, "expected/Log_ins.pddl" );
	}

	void scheduleTest() {
		Domain dom( "domains/Sched_dom.pddl" );
		Instance ins( dom, "domains/Sched_ins.pddl" );

		checkEqual( dom, "expected/Sched_dom.pddl" );
		checkEqual( ins, "expected/Sched_ins.pddl" );
	}

	void elevatorTest() {
		Domain dom( "domains/Elev_dom.pddl" );
		Instance ins( dom, "domains/Elev_ins.pddl" );

		checkEqual( dom, "expected/Elev_dom.pddl" );
		checkEqual( ins, "expected/Elev_ins.pddl" );
	}

	void temporalTest() {
		Domain dom( "domains/Mapana_dom.pddl" );
		Instance ins( dom, "domains/Mapana_ins.pddl" );

		checkEqual( dom, "expected/Mapana_dom.pddl" );
		checkEqual( ins, "expected/Mapana_ins.pddl" );
	}
};



TEST_F(DomainTests, LogisticsTest)
{
	logisticsTest();
}

TEST_F(DomainTests, ScheduleTest)
{
	scheduleTest();
}

TEST_F(DomainTests, ElevatorTest)
{
	elevatorTest();
}

TEST_F(DomainTests, TemporalTest)
{
	temporalTest();
}

int main(int argc, char **argv) 
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


//TEST(DomainTests, )

//REGISTER_FIXTURE( DomainTests )

