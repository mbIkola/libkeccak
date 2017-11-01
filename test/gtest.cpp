//
// Created by mbikola on 10/31/17.
//

#include "gtest/gtest.h"
#include <string>
#include <fstream>

#include "keccak/keccak.h"


#define c2i(x) (( x>='0' && x<='9' ) ? x-'0' :  ( x>='a' && x<='f' ? 10+x-'a' : 10+x-'A'))

using namespace std;

template < size_t DIGEST_SIZE,  char const*fixture_file_csv >
class HashShitFuncTestTemplate: public ::testing::Test {
protected:

    size_t digest_size = DIGEST_SIZE;

    uint8_t *input_binary = NULL;
    uint8_t *digest_binary = NULL;

    // tuple of message => expected digest
    typedef std::tr1::tuple<string, string> StringPair;
    vector<StringPair> fixtures;


    virtual void SetUp() {
        digest_binary = new uint8_t[digest_size];

        ifstream infile(fixture_file_csv);
        ASSERT_FALSE(infile.bad());

        string input, expected, line, digest;

        while ( ! getline(infile, line).eof()) {
            stringstream ss(line);
            getline(ss, input, ',');
            getline(ss, expected, ',');

            ASSERT_TRUE(input.length() % 2 == 0);
            //ASSERT_EQ(input.length(), digest_size * 2);

            fixtures.push_back(StringPair(input, expected));
        }

    }

    virtual string calc(string input) = 0;

    virtual void TearDown() {
        if ( digest_binary ) delete[] digest_binary;
        if ( input_binary ) delete[] input_binary;

        digest_binary = NULL;
    }


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

char keccak1600_fixture[] = "data/keccak1600.csv";
class Keccak1600Test : public HashShitFuncTestTemplate<200, keccak1600_fixture> {

protected:
    string calc(string input) {
        input_binary = hex2bin(input);
        keccak1600(input_binary, digest_size, digest_binary);
        return bin2hex(digest_binary, digest_size);
    }
};


char keccakf_fixture[] = "data/keccakf.csv";
class KeccakFTest :  public HashShitFuncTestTemplate<200, keccakf_fixture> {
    string calc(string input) {
        input_binary = hex2bin(input);
        keccakf((uint64_t *)input_binary, 24); // inplace; no need of dig
        return bin2hex(input_binary, digest_size);
    }
};

TEST_F(Keccak1600Test, defaultFixturesTest) {
    string inputMessage, expectedDigest;

    for ( auto const &pair : fixtures) {
        std::tie(inputMessage, expectedDigest) = pair;
        EXPECT_EQ(inputMessage, expectedDigest);
    }
}

TEST_F(KeccakFTest, defaultFixturesTest) {
    string inputMessage, expectedDigest;

    for ( auto const &pair : fixtures) {
        std::tie(inputMessage, expectedDigest) = pair;
        EXPECT_EQ(inputMessage, expectedDigest);
    }
}



TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
    // fuck life, the universe and everything related to cryptography and blockchains
    EXPECT_NE("42", "Answer to the Ultimate Question of Life, the Universe, and Everything");
}


int main(int args, char **argv) {
    ::testing::InitGoogleTest(&args, argv);
    return RUN_ALL_TESTS();
}