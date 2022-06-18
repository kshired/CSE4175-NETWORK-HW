#include <fstream>
#include <iostream>
#include <string>
#include <bitset>

using namespace std;
int error = 0;

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

char crc(string target, string generator, int size) {
    bool flag = false;
    int len = generator.length();

    if (size == 4) {
        string target1 = target.substr(0, len + 3);
        string target2 = target.substr(len + 3, len + 3);

        string remainder1 = mod2div(target1, generator);
        string remainder2 = mod2div(target2, generator);

        for (char c: remainder1) {
            if (c == '1') {
                flag = true;
                break;
            }
        }

        if (flag) {
            error++;
        }

        flag = false;

        for (char c: remainder2) {
            if (c == '1') {
                flag = true;
                break;
            }
        }

        if (flag) {
            error++;
        }

        return str_to_char(target1.substr(0, 4) + target2.substr(0, 4));
    } else {
        string remainder = mod2div(target, generator);
        for (char c: remainder) {
            if (c == '1') {
                flag = true;
                break;
            }
        }

        if (flag) {
            error++;
        }

        return str_to_char(target.substr(0, 8));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];
    string result_file = argv[3];
    string generator = argv[4];
    string dataword_size = argv[5];

    int dataword_size_int;

    if (dataword_size.compare("4") == 0) {
        dataword_size_int = 4;
    } else if (dataword_size.compare("8") == 0) {
        dataword_size_int = 8;
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

    ofstream result(result_file.data());

    if (!result.is_open()) {
        printf("result file open error.\n");
        result.close();
        return 1;
    }

    char c;
    input.get(c);
    string padding = char_to_str(c);

    string tmp = "";
    int frame = 0;
    int codeword_len = dataword_size_int == 4 ? (6 + 2 * generator.length()) : (7 + generator.length());
    int step = dataword_size_int == 4 ? 2 : 1;

    while (input.get(c)) {
        tmp += char_to_str(c);
    }

    int padding_size = tmp.length() - (tmp.length() / codeword_len * codeword_len);

    tmp = tmp.substr(padding_size, tmp.length() - padding_size);

    for (int pos = 0; pos < tmp.length(); pos += codeword_len) {
        output << crc(tmp.substr(pos, codeword_len), generator, dataword_size_int);
        frame += step;
    }

    result << frame << " " << error << "\n";


    input.close();
    output.close();
    result.close();
    return 0;
}