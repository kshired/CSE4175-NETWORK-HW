#include <fstream>
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

string my_xor(string a, string b) {
    string res = "";

    for (int i = 1; i < b.length(); i++) {
        res += to_string(a[i] ^ b[i]);
    }

    return res;
}

string mod2div(string dividend, string divisor) {
    int cur = divisor.length();
    string res = dividend.substr(0, cur);

    for (; cur <= dividend.length(); cur++) {
        if (res[0] == '1') {
            if (cur == dividend.length()) {
                res = my_xor(divisor, res);
            } else {
                res = my_xor(divisor, res) + dividend[cur];
            }
        } else {
            if (cur == dividend.length()) {
                res = my_xor(string(cur, '0'), res);
            } else {
                res = my_xor(string(cur, '0'), res) + dividend[cur];
            }
        }
    }

    return res;
}

string char_to_str(char ch) {
    bitset<8> temp(ch);
    return temp.to_string();
}

char str_to_char(string s) {
    return stoi(s, 0, 2);
}


string crc(char c, string generator, int n) {
    int len = generator.length();
    string tmp = char_to_str(c);
    string res;

    if (n == 4) {
        string target1 = tmp.substr(0, 4);
        string target2 = tmp.substr(4, 4);

        string remainder1 = mod2div(target1 + string(len - 1, '0'), generator);
        string remainder2 = mod2div(target2 + string(len - 1, '0'), generator);
        res = target1 + remainder1 + target2 + remainder2;

    } else {
        string target = tmp;
        string remainder = mod2div(target + string(len - 1, '0'), generator);
        res = target + remainder;
    }

    return res;
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("usage: ./crc_encoder input_file output_file generator dataword_size\n");
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];
    string generator = argv[3];
    string dataword_size = argv[4];
    int size;

    if (dataword_size.compare("4") == 0) {
        size = 4;
    } else if (dataword_size.compare("8") == 0) {
        size = 8;
    } else {
        printf("dataword size must be 4 or 8.\n");
        return 1;
    }

    ifstream input(input_file.data());

    if (!input.is_open()) {
        printf("input file open error.\n");
        input.close();
        return 1;
    }

    ofstream output(output_file.data());

    if (!output.is_open()) {
        printf("output file open error.\n");
        output.close();
        return 1;
    }

    char c;
    string res = "";
    while (input.get(c)) {
        res += crc(c, generator, size);
    }

    if (res.length() % 8 != 0) {
        int padding_size = 8 - (res.length() % 8);
        res = char_to_str(padding_size) + res;
        res = string(padding_size, '0') + res;
    } else {
        res = char_to_str(0) + res;
    }

    for (int pos = 0; pos < res.length(); pos += 8) {
        output << str_to_char(res.substr(pos, 8));
    }

    input.close();
    output.close();
    return 0;
}