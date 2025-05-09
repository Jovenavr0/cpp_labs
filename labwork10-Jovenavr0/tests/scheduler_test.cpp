#include <lib/scheduler.h>
#include <gtest/gtest.h>
#include <valarray>

TEST(Scheduler, OneTaskOnlyLambdaInt) {
	sched::TTaskScheduler shed;
	auto id1 = shed.add([](){return 1 * 2 * 3 * 4 * 5;});
	shed.executeAll();
	ASSERT_EQ(120, shed.getResult<int>(id1));
}

TEST(Scheduler, OneTaskOnlyLambdaString) {
	sched::TTaskScheduler shed;
	auto id1 = shed.add([](){return "everything ok";});
	shed.executeAll();
	ASSERT_EQ("everything ok", shed.getResult<std::string>(id1));
}

TEST(Scheduler, OneTaskWith1Argument) {
	sched::TTaskScheduler shed;
	auto id1 = shed.add([](int a){return std::sqrt(a);}, 51984);
	shed.executeAll();
	ASSERT_EQ(228, shed.getResult<int>(id1));
}

TEST(Scheduler, OneTaskWith2StaticArgument) {
	sched::TTaskScheduler shed;
	int a = rand() % 100;
	int b = rand() % 100;
	auto id1 = shed.add([](int a, int b){return a + b;}, a, b);
	shed.executeAll();
	ASSERT_EQ((a + b), shed.getResult<int>(id1));
}

TEST(Scheduler, OneTaskWith1DinamicArgument) {
	sched::TTaskScheduler shed;
	int a = rand() % 100;
	int b = 2;
	auto id1 = shed.add([](int a, int b){return a + b;}, a, &b);
	b = 55;
	shed.executeAll();
	ASSERT_EQ((a + 55), shed.getResult<int>(id1));
}

TEST(Scheduler, OneTaskWith2DinamicArgument) {
	sched::TTaskScheduler shed;
	int a = rand() % 100;
	int b = 2;
	auto id1 = shed.add([](int a, int b){return a + b;}, &a, &b);
	b = 55;
	a = 700;
	shed.executeAll();
	ASSERT_EQ((700 + 55), shed.getResult<int>(id1));
}

TEST(Scheduler, TwoWithoutConnection) {
	sched::TTaskScheduler shed;
	bool flag = true;
	int a = 2025;
	int b = rand() % 1000;
	auto id1 = shed.add([](int a){return std::sqrt(a);}, a);
	auto id2 = shed.add([](int a, int b){return a + b;}, a, b);
	shed.executeAll();
	if ((2025 + b) != shed.getResult<int>(id2) || ((45 != shed.getResult<double>(id1)))) {
		flag = false;
	}
	ASSERT_EQ(flag, true);
}

TEST(Scheduler, TwoTaskWithConnection) {
	sched::TTaskScheduler shed;
	int a = 2025;
	auto id1 = shed.add([](int a){return std::sqrt(a);}, a);
	auto id2 = shed.add([](int a, int b){return a + b;}, a, shed.getFutureResult<double>(id1));
	shed.executeAll();
	ASSERT_EQ((2025 + 45), shed.getResult<int>(id2));
}

TEST(Scheduler, ThreeTaskConection) {
	sched::TTaskScheduler shed;
	int a = 2025;

	auto id1 = shed.add([](int a){return std::sqrt(a);}, a);
	auto id2 = shed.add([](int a, int b){return a + b;}, a, shed.getFutureResult<double>(id1));
	auto id3 = shed.add([](int a, int b){return a + b;}, a, shed.getFutureResult<int>(id2));

	shed.executeAll();

	ASSERT_EQ((2025 + 45 + 2025), shed.getResult<int>(id3));
}

TEST(Scheduler, TwoTaskConnectrionWithOne){
	sched::TTaskScheduler shed;
	int a = 2025;
	bool flag = true;

	auto id1 = shed.add([](int a){return std::sqrt(a);}, a);
	auto id2 = shed.add([](int a, int b){return a + b;}, a, shed.getFutureResult<double>(id1));
	auto id3 = shed.add([](int a, int b){return a * b;}, a, shed.getFutureResult<double>(id1));

	shed.executeAll();

	if (((2025 + 45) != shed.getResult<int>(id2)) || ((2025 * 45) != shed.getResult<int>(id3))) {
		flag = false;
	}

	ASSERT_EQ(flag, true);
}

TEST(Scheduler, Discriminant) {
	sched::TTaskScheduler scheduler;

	float a = 1;
	float b = -2;
	float c = 0;
	bool flag = true;

	auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, b);
	auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
	auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
	auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
	auto id5 = scheduler.add([](float a, float v){return v / (2 * a);}, a, scheduler.getFutureResult<float>(id3));
	auto id6 = scheduler.add([](float a, float v){return v / (2 * a);},a, scheduler.getFutureResult<float>(id4));

	scheduler.executeAll();

 	if ((2 != scheduler.getResult<int>(id5)) || (0 != scheduler.getResult<int>(id6))) {
		 flag = false;
	}

	ASSERT_EQ(flag, true);
}

TEST(Scheduler, OperationInTask) {
	sched::TTaskScheduler shed;

	auto id1 = shed.add([](int a){return std::sqrt(a);}, (2000 + 25));

	shed.executeAll();

	ASSERT_EQ(45, shed.getResult<int>(id1));
}

TEST(Scheduler, TasksFirstPointer) {
	sched::TTaskScheduler shed;
	int a = 2025;

	auto id1 = shed.add([](int a){return std::sqrt(a);}, a);
	auto id2 = shed.add([](int a, int b) {return b + a;}, shed.getFutureResult<double>(id1), 1980);

	shed.executeAll();

	ASSERT_EQ(a, shed.getResult<int>(id2));
}

TEST(Scheduler, StringTasks) {
	sched::TTaskScheduler shed;
	std::string a{"hello"};

	auto id1 = shed.add([](unsigned char a) -> unsigned char {return std::toupper(a);}, &a[0]);
	auto id2 = shed.add([](unsigned char a) -> unsigned  char {return std::toupper(a);}, &a[1]);
	auto id3 = shed.add([](unsigned char a) -> unsigned  char {return std::toupper(a);}, &a[2]);
	auto id4 = shed.add([](unsigned char a) -> unsigned char{return std::toupper(a);}, &a[3]);
	auto id5 = shed.add([](unsigned char a) -> unsigned char {return std::toupper(a);}, &a[4]);

	shed.executeAll();

	std::string answer;
	answer += shed.getResult<unsigned char>(id1);
	answer += shed.getResult<unsigned char>(id2);
	answer += shed.getResult<unsigned char>(id3);
	answer += shed.getResult<unsigned char>(id4);
	answer += shed.getResult<unsigned char>(id5);

	ASSERT_EQ("HELLO", answer);

}
