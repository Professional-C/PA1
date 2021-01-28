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
    
    SECTION("pic.jpg. test")
    {
        char test[] = "qwertyuiopq" "aaa" "bcde" "\x00\x00";
        char expected[] = "\xF5" "qwertyuiopq" "\x03" "a" "\xFC" "bcde" "\x02" "\x00";

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
   
    SECTION("Long negative run")
    {

        char test[] = "\x81" "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw" "\xFD" "xyz";
        char expected[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";

        bool result = RunDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);

        REQUIRE(result);
    }
    
    // TA TESTS
    
    SECTION("Basic mix run") //not sure if correct
        {
            char test[] = "aabcccdefg"; //2 a 1 b 3 c -4 d e f g
            char expected[] = "\x02" "a" "\x01" "b" "\x03" "c" "\xFC" "d" "e" "f" "g";
            
            bool result = RunCompressionTest(test, sizeof(test) - 1,
                                             expected, sizeof(expected) - 1);
            REQUIRE(result);
        }
    

   SECTION("Overflow negative run followed by short positive") //not sure if correct
   {
       char test[204];
       for(int i = 0; i < 200; i++){
           test[i] = 'a' + i%3;
       }
       test[200] = 'e';
       test[201] = 'e';
       test[202] = 'e';
       
       char expected[205];
       for(int i = 1; i <= 127; i++){
           expected[i] = 'a' + (i - 1)%3;
       }
       for(int i = 129; i < 202; i++){
           expected[i] = 'a' + (i - 2)%3;
       }
       expected[0] = '\x81'; //-127 .. -73 ... 3
       expected[128] = '\xB7';
       expected[202] = '\x03';
       expected[203] = 'e';

       bool result = RunCompressionTest(test, sizeof(test) - 1,
                                        expected, sizeof(expected) - 1);
       REQUIRE(result);
   }
SECTION("Overflow mix run")
    {
        char test[201];
        for(int i = 0; i < 198; i++){
            test[i] = 'a';
        }
        test[198] = 'b';
        test[199] = 'c';
        char expected[] = "\x7F" "a" "\x47" "a" "\xFE" "b" "c"; //127 a 71 a -2 b c
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Overflow mix run2")
    {
        char test[201];
        for(int i = 0; i < 199; i++){
            test[i] = 'a';
        }
        test[199] = 'b';
        char expected[] = "\x7F" "a" "\x48" "a" "\x01" "b" ; //127 a 72 a 1 b
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }


	// TODO: Add more test case sections!!
}

TEST_CASE("Command line arguments", "[student]")
{
	SECTION("instructions.txt.rl1")
	{
		const char* argv[] = {
			"tests/tests",
			"data/instructions.txt.rl1"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
		REQUIRE(result);
	}
}

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

TEST_CASE("File decompression", "[student]")
{
	RleFile r;
	SECTION("works.bmp.rl1")
	{
		r.ExtractArchive("data/works.bmp.rl1");
		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
		REQUIRE(result);
	}
	SECTION("xkcd.bmp.rl1")
	{
		r.ExtractArchive("data/xkcd.bmp.rl1");
		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
		REQUIRE(result);
	}
	SECTION("logo.png.rl1")
	{
		r.ExtractArchive("data/logo.png.rl1");
		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
		REQUIRE(result);
	}
}

TEST_CASE("RLE Compression TA", "[student]")
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

    SECTION("Basic positive run2")
    {
        char test[] = "abbccd";
        char expected[] = "\x01" "a" "\x02" "b" "\x02" "c" "\x01" "d"; //1 a 2 b 2 c 1 d
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
     SECTION("Overflow positive run")
     {
     char test[128];
     for(int i = 0; i < 127; i++){
     test[i] = 'a';
     }
     char expected[] = "\x7F" "a";
     
     bool result = RunCompressionTest(test, sizeof(test) - 1,
     expected, sizeof(expected) - 1);
     REQUIRE(result);
     }
     
    // TODO: Add more test case sections!!
    SECTION("Basic negative run")
    {
        char test[] = "abcdefg";
        char expected[] = "\xF9" "a" "b" "c" "d" "e" "f" "g"; //-7 a b c d e f g
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
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

    SECTION("Overflow negative run") //not sure if correct
    {
        char test[201];
        for(int i = 0; i < 200; i++){
            test[i] = 'a' + i%3;
        }
        char expected[203];
        for(int i = 1; i <= 127; i++){
            expected[i] = 'a' + (i - 1)%3;
        }
        for(int i = 129; i < 202; i++){
           expected[i] = 'a' + (i - 2)%3;
        }
        expected[0] = '\x81'; //-127 .. -73 ...
        expected[128] = '\xB7';
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Overflow negative run followed by short positive") //not sure if correct
    {
        char test[204];
        for(int i = 0; i < 200; i++){
            test[i] = 'a' + i%3;
        }
        test[200] = 'e';
        test[201] = 'e';
        test[202] = 'e';
        
        char expected[205];
        for(int i = 1; i <= 127; i++){
            expected[i] = 'a' + (i - 1)%3;
        }
        for(int i = 129; i < 202; i++){
            expected[i] = 'a' + (i - 2)%3;
        }
        expected[0] = '\x81'; //-127 .. -73 ... 3
        expected[128] = '\xB7';
        expected[202] = '\x03';
        expected[203] = 'e';

        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Overflow mix run")
    {
        char test[201];
        for(int i = 0; i < 198; i++){
            test[i] = 'a';
        }
        test[198] = 'b';
        test[199] = 'c';
        char expected[] = "\x7F" "a" "\x47" "a" "\xFE" "b" "c"; //127 a 71 a -2 b c
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Overflow mix run2")
    {
        char test[201];
        for(int i = 0; i < 199; i++){
            test[i] = 'a';
        }
        test[199] = 'b';
        char expected[] = "\x7F" "a" "\x48" "a" "\x01" "b" ; //127 a 72 a 1 b
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
   
    SECTION("Extra mix run1")
    {
        char test[] = "abcccdefg"; //-2 a b 3 c -4 d e f g
        char expected[] = "\xFE" "a" "b" "\x03" "c" "\xFC" "d" "e" "f" "g";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Extra mix run2")
    {
        char test[] = "aaabcdefg"; //3 a -6 b c d e f g
        char expected[] = "\x03" "a" "\xFA" "b" "c" "d" "e" "f" "g";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Short negative run")
    {
        char test[] = "qwertyuiopasdfghjklzxcvbnm"; //-26
        char expected[] = "\xE6" "qwertyuiopasdfghjklzxcvbnm";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
}

TEST_CASE("RLE Decompression TA", "[student]")
{
    SECTION("Basic positive run")
    {
        char test[] = "\x28" "x";
        char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    // TODO: Add more test case sections!!
    SECTION("Basic negative run")
    {
        char test[] = "\xFD" "a" "b" "c"; //-3 a b c
        char expected[] = "abc";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Basic mix run")
    {
        char test[] = "\xFD" "a" "b" "c" "\x03" "d" "\x01" "e"; //-3 a b c d d d e
        char expected[] = "abcddde";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Overflow run")
    {
        char test[] = "\x7F" "a" "\x01" "c" "\xFD" "a" "b" "c"; //127 a 1 c -3 a b c
        char expected[132];
        for (int i = 0; i < 127; i++) {
            expected[i] = 'a';
        }
        expected[127] = 'c';
        for (int i = 0; i < 3; i++) {
            expected[128 + i] = 'a' + i;
        }
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
   
}
//
//TEST_CASE("Command line arguments TA", "[student]")
//{
//    SECTION("instructions.txt.rl1")
//    {
//        const char* argv[] = {
//            "tests/tests",
//            "data/instructions.txt.rl1"
//        };
//        ProcessCommandArgs(2, argv);
//        bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
//        REQUIRE(result);
//    }
//}
//
//TEST_CASE("File compression TA", "[student]")
//{
//    RleFile r;
//    SECTION("rle.bmp")
//    {
//        r.CreateArchive("data/rle.bmp");
//        bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
//        REQUIRE(result);
//    }
//    SECTION("pic.jpg")
//    {
//        r.CreateArchive("data/pic.jpg");
//        bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
//        REQUIRE(result);
//    }
//    SECTION("Conquest.ogg")
//    {
//        r.CreateArchive("data/Conquest.ogg");
//        bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
//        REQUIRE(result);
//    }
//}
//
//TEST_CASE("File decompression TA", "[student]")
//{
//    RleFile r;
//    SECTION("works.bmp.rl1")
//    {
//        r.ExtractArchive("data/works.bmp.rl1");
//        bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
//        REQUIRE(result);
//    }
//    SECTION("xkcd.bmp.rl1")
//    {
//        r.ExtractArchive("data/xkcd.bmp.rl1");
//        bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
//        REQUIRE(result);
//    }
//    SECTION("logo.png.rl1")
//    {
//        r.ExtractArchive("data/logo.png.rl1");
//        bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
//        REQUIRE(result);
//    }
//}
