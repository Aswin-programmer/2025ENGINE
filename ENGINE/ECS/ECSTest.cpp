#include "ECSTest.h"

FlecsTest::FlecsTest()
{
}

FlecsTest::~FlecsTest()
{
}

void FlecsTest::Test()
{

	flecs::entity e1 = world.entity("Bod");
	std::cout << "Entity name : " << e1.name() << std::endl;

	flecs::entity e2 = world.entity("CAd");
	std::cout << "Entity name : " << e2.name() << std::endl;

	world.component<Transfrom1>();

	e1.set<Transfrom1>({ 10, 20 });
	e1.set<Transfrom2>({ 130, 30 });

	auto query = world.query<Transfrom1, Transfrom2>();

	query.each([](flecs::entity e, Transfrom1& t1, Transfrom2& t2) {
		// Use components directly
		std::cout << "Entity " << e.name() << " has both components\n";
		});

}

