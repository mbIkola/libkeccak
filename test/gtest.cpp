//
// Created by mbikola on 10/31/17.
//

#include "gtest/gtest.h"
#include <string>
#include <fstream>

#include "keccak/keccak.h"


/**
 * char 2 int macro.
 */
#define c2i(x) (( x>='0' && x<='9' ) ? x-'0' :  ( x>='a' && x<='f' ? 10+x-'a' : 10+x-'A'))

using namespace std;

// tuple of message => expected digest
typedef std::tr1::tuple<string, string> StringPair;


class Hex2BinTrait {
public:
    inline uint8_t *hex2bin( string input) {

        uint8_t * res = new uint8_t[input.length() / 2];
        const char *i = input.c_str();
        int offset = 0;

        while(*i) { /// expecting it ends with '\0'
            res[offset++] = c2i(*i) << 4  | c2i(*(i+1));
            i+=2; // two chars per byte
        }

        return res;
    }

    inline string bin2hex( uint8_t *bin, size_t length ) {
        static const char hextable[] = "0123456789abcdef";
        string hex;

        hex.reserve(length * 2 + 1);
        size_t i = 0;

        while ( i < length ) {
            hex += hextable[ (bin[i] & 0x0f0) >> 4];
            hex += hextable[ bin[i] & 0x0f ];
            i++;
        }

        return hex;
    }
};


class HashShitFuncTestTemplate: public ::testing::TestWithParam<StringPair>, public Hex2BinTrait {
protected:

    size_t digest_size ;

    uint8_t *input_binary = NULL;
    uint8_t *digest_binary = NULL;


    virtual void SetUp() {
        digest_binary = new uint8_t[digest_size];
    }
    virtual void TearDown() {
        if ( digest_binary ) delete[] digest_binary;
        if ( input_binary ) delete[] input_binary;

        digest_binary = NULL;
    }


    virtual string calc(string input) = 0;

};


class Keccak1600Test : public HashShitFuncTestTemplate {
public:
    Keccak1600Test() : HashShitFuncTestTemplate() { digest_size = 200; };
protected:
    string calc(string input) {
        input_binary = hex2bin(input);
        keccak1600(input_binary, digest_size, digest_binary);
        return bin2hex(digest_binary, digest_size);
    }
};


class KeccakFTest :  public HashShitFuncTestTemplate {
public:
    KeccakFTest() : HashShitFuncTestTemplate() { digest_size = 200; };
protected:
    string calc(string input) {
        input_binary = hex2bin(input);
        keccakf((uint64_t *)input_binary, 24); // inplace; no need of dig
        return bin2hex(input_binary, digest_size);
    }
};





TEST_P(KeccakFTest, defaultFixturesTest) {

    string inputMessage, expectedDigest;
    std::tie(inputMessage, expectedDigest) = GetParam();

    string calculatedDigest = calc(inputMessage);

    EXPECT_EQ(calculatedDigest, expectedDigest);
}


TEST_P(Keccak1600Test, defaultFixturesTest) {
    string inputMessage, expectedDigest;
    std::tie(inputMessage, expectedDigest) = GetParam();

    string calculatedDigest = calc(inputMessage);

    EXPECT_EQ(calculatedDigest, expectedDigest);
}




TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
    // fuck life, the universe and everything related to cryptography and blockchains
    EXPECT_NE("42", "Answer to the Ultimate Question of Life, the Universe, and Everything");
}

class FixturesCSVLoader {
public:
    static vector<StringPair> load( const char *path_to_csv_file ) {


        ifstream infile(path_to_csv_file);
        assert(infile.good());

        string input, expected, line, digest;
        vector<StringPair> fixtures;

        while ( ! getline(infile, line).eof()) {
            if ( line[line.size()-1] == '\r')  {
                line.resize(line.size() - 1); // dirty hach against CRLF/CR/LF line ending handling in ::std::getline
            }

            stringstream ss(line);
            getline(ss, input, ',');
            getline(ss, expected, ',');

            assert(input.length() % 2 == 0);
            //ASSERT_EQ(input.length(), digest_size * 2);

            fixtures.push_back(StringPair(input, expected));
        }
        return fixtures;

    }
};

auto keccakfFixtures = FixturesCSVLoader::load("data/keccakf.csv");
INSTANTIATE_TEST_CASE_P(defaultFixturesTest, KeccakFTest, ::testing::ValuesIn(keccakfFixtures));

auto keccak1600Fixtures = FixturesCSVLoader::load("data/keccak1600.csv");
INSTANTIATE_TEST_CASE_P(defaultFixturesTest, Keccak1600Test, ::testing::ValuesIn(keccak1600Fixtures));

int main(int args, char **argv) {
    ::testing::InitGoogleTest(&args, argv);


    return RUN_ALL_TESTS();
}