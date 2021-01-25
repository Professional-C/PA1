#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"

// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
							   char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
								 char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);

TEST_CASE("RLE Compression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
		"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
    
    SECTION("A very long positive run larger than max size")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7F" "a" "\x21" "a";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }

    SECTION("Basic negative run")
    {
        char test[] = "aaaaaabcde";
        char expected[] = "\x6" "a" "\xFC" "b" "c" "d" "e";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
    
    SECTION("Alternating positive and negative runs")
    {
        char test[] = "aaaaapqpqpaaaaapqpqp";
        char expected[] = "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }

    SECTION("Single char")
    {
        char test[] = "a";
        char expected[] = "\x01" "a";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

    }
    
    SECTION("Long negative run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xFD" "xyz";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }

    SECTION("Long positive run")
    {
        char test[] = "aaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccdddddddddd";
        char expected[] = "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
    
    SECTION("Alternating positive and negative runs")
    {
        char test[] = "aaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqp";
        char expected[] = "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p""\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
    
    SECTION("Long alternating positive and negative runs")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbabcdeaaaaabbbbbabcde";
        char expected[] = "\x7F" "a" "\x21" "a" "\x05" "b" "\xFB" "abcde" "\x05" "a" "\x05" "b" "\xFB" "abcde";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }


   
    SECTION("Short positive run")
    {
        char test[] = "aaa";
        char expected[] = "\x03" "a";

        bool result = RunCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
    

	// TODO: Add more test case sections!!
}

TEST_CASE("RLE Decompression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

		bool result = RunDecompressionTest(test, sizeof(test) - 1,
										   expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
    
    SECTION("Small negative run")
    {
        char test[] = "\xFB" "abcde";
        char expected[] = "abcde";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }

    SECTION("Basic negative run")
    {
        char test[] = "\xF6" "abcdeabcde";
        char expected[] = "abcdeabcde";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }

    SECTION("Long positive run")
    {
        char test[] = "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d" "\xA" "a" "\xA" "b" "\xA" "c" "\xA" "d";
        char expected[] = "aaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccdddddddddd";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }

    SECTION("Long alternating positive and negative runs")
    {
        char test[] = "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p""\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p" "\x05" "a" "\xFB" "p" "q" "p" "q" "p";
        char expected[] =  "aaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqpaaaaapqpqp";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Basic mix run") //not sure if correct
        {
            char test[] = "aabcccdefg"; //2 a 1 b 3 c -4 d e f g
            char expected[] = "\x02" "a" "\x01" "b" "\x03" "c" "\xFC" "d" "e" "f" "g";
            
            bool result = RunCompressionTest(test, sizeof(test) - 1,
                                             expected, sizeof(expected) - 1);
            REQUIRE(result);
        }
    
    SECTION("Long negative run")
    {

        char test[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xFD" "xyz";
        char expected[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";

        bool result = RunDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
	// TODO: Add more test case sections!!
}

//TEST_CASE("Command line arguments", "[student]")
//{
//	SECTION("instructions.txt.rl1")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"data/instructions.txt.rl1"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
//		REQUIRE(result);
//	}
//}
//
TEST_CASE("File compression", "[student]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("data/rle.bmp");
		bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("data/pic.jpg");
		bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("data/Conquest.ogg");
		bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}

//TEST_CASE("File decompression", "[student]")
//{
//	RleFile r;
//	SECTION("works.bmp.rl1")
//	{
//		r.ExtractArchive("data/works.bmp.rl1");
//		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
//		REQUIRE(result);
//	}
//	SECTION("xkcd.bmp.rl1")
//	{
//		r.ExtractArchive("data/xkcd.bmp.rl1");
//		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
//		REQUIRE(result);
//	}
//	SECTION("logo.png.rl1")
//	{
//		r.ExtractArchive("data/logo.png.rl1");
//		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
//		REQUIRE(result);
//	}
//}
//
