#include <gtest/gtest.h>

#include <big_int.h>
#include <client_logger.h>
#include <client_logger_builder.h>
#include <operation_not_supported.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

//TEST(my_test, t1)
//{
//    std::vector<unsigned int> vec1{0, 1, 2, 3, 4, 5};
//
//    big_int num1(vec1);
//
//    std::vector<unsigned int> vec2{3, 2, 1};
//
//    big_int num2(vec2);
//
//    auto res_div = num1 / num2;
//
//    std::string val_div = res_div.to_string();
//
//    auto res_mode = num1 % num2;
//
//    auto val_mod = res_mode.to_string();
//
//    EXPECT_EQ(val_div, "396140812460641223525462442003");
//
//    EXPECT_EQ(val_mod, "18446743914795761607");
//}

TEST(positive_tests, test1)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("32850346459076457453464575686784654", 10);
    big_int bigint_2("423534596495087569087908753095322", 10);
    std::cout << "B1 = " << bigint_1 << std::endl;
    std::cout << "B2 = " << bigint_2 << std::endl;
    big_int result_of_sum = bigint_1 + bigint_2;
    
    std::stringstream ss;
    ss << result_of_sum;
    std::string result_string = ss.str();
    
    EXPECT_EQ(result_string, "33273881055571545022552484439879976");
    
    delete logger;
}

TEST(positive_tests, test2)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("32850346459076457453464575686784654");
    big_int bigint_2("0000042353459649508756908790875309532245366457546765745645647567575");
    std::cout << "B1 = " << bigint_1 << std::endl;
    std::cout << "B2 = " << bigint_2 << std::endl;

    big_int result_of_sub = bigint_1 - bigint_2;
    
    std::stringstream ss;
    ss << result_of_sub;
    std::string result_string = ss.str();
    std::cout<<"result_string = " << result_string << std::endl;
    
    EXPECT_TRUE(result_string == "-42353459649508756908790875276681898907381089312281069960782921");
    
    delete logger;
}

TEST(positive_tests, test3)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });

    big_int bigint_1("32850346459076457453464575686784654");
    big_int bigint_2("12342357553253");

    big_int result_of_remainder = bigint_1 % bigint_2;

    std::stringstream ss;
    ss << result_of_remainder;
    std::string result_string = ss.str();

    EXPECT_TRUE(result_string == "3232571319826");

    delete logger;
}

TEST(positive_tests, test4)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("-54357893745346457544353");
    std::cout << bigint_1 << std::endl;
    big_int bigint_2("-54357893745346457544354");
    
    EXPECT_TRUE(bigint_1 > bigint_2);
    
    delete logger;
}

TEST(positive_tests, test5)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("423895435432");
    big_int bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 >= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test6)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("-423895435432312432534645756753");
    big_int bigint_2("0");
    
    EXPECT_TRUE(bigint_1 < bigint_2);
    
    delete logger;
}

TEST(positive_tests, test7)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("4238954354324222200000000");
    big_int bigint_2("4238954354324222222222234");
    
    EXPECT_TRUE(bigint_1 <= bigint_2);
    
    delete logger;
}

TEST(positive_tests, test8)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("4238954354321");
    big_int bigint_2("423895435432");
    
    EXPECT_TRUE(bigint_1 != bigint_2);
    
    delete logger;
}

TEST(positive_tests, test9)
{
    logger *logger = create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "bigint_logs.txt",
                logger::severity::information
            },
        });
    
    big_int bigint_1("-00000044234235347865897389456748953795739648996453238954354321");
    big_int bigint_2("-00000044234235347865897389456748953795739648996453238954354321");
    
    EXPECT_TRUE(bigint_1 == bigint_2);
    
    delete logger;
}

int main(
    int argc,
    char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}