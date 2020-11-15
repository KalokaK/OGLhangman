//
// Created by kaloka on 15/11/2020.
//

#include <gtest/gtest.h>
#include <helpers.h>

void fooSingleArg(int val) { printf("from non member fun : %i\n", val); }
struct memberFooSingleArgHolder{
    int i = 1;
    void memberFooSingleArg(int val) { i += val; printf("from member fun : %i\n", i); }
};
TEST(eventSystemTest, singleArgEvent) {
    auto memberFooHolder = memberFooSingleArgHolder();
    auto event1 = new events::event<int>();
    *event1 += events::handler(fooSingleArg);
    *event1 += events::handler(&memberFooSingleArgHolder::memberFooSingleArg, &memberFooHolder);
    (*event1)(5);
    *event1 -= events::handler(fooSingleArg);
    *event1 -= events::handler(&memberFooSingleArgHolder::memberFooSingleArg, &memberFooHolder);
    (*event1)(10);
    delete event1;
}

void fooMultiArg(int val, float val2) { printf("from non member fun : %i : %f\n", val, val2); }
struct memberFooMultiArgHolder{
    int i = 1;
    float y = 1;
    void memberFooMultiArg(int val, float val2) {
        i += val; y += val2;
        printf("from member fun : %i : %f\n", i, y);
    }
};

TEST(eventSystemTest, multiArgEvent) {
    auto memberFooHolder = memberFooMultiArgHolder();
    auto event1 = new events::event<int, float>();
    *event1 += events::handler(fooMultiArg);
    *event1 += events::handler(&memberFooMultiArgHolder::memberFooMultiArg, &memberFooHolder);
    (*event1)(5, 10.);
    *event1 -= events::handler(fooMultiArg);
    *event1 -= events::handler(&memberFooMultiArgHolder::memberFooMultiArg, &memberFooHolder);
    (*event1)(10, 20.);
    delete event1;
}