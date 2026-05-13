#include <iostream>
#include <fstream>
#include <cstdint>
#include <Windows.h>
#include <list>
#include <utf8.h>
#include <string>
#include "карта.hpp"

using namespace std;


bool isLetter(uint32_t cp) {
    return
        (cp >= 'A' && cp <= 'Z') ||
        (cp >= 'a' && cp <= 'z') ||
        (cp >= 0x0410 && cp <= 0x044F) || // А-я
        cp == 0x0401 || cp == 0x0451;     // Ё ё
}

bool isDigit(uint32_t cp) {
    return cp >= '0' && cp <= '9';
}

bool isSpace(uint32_t cp) {
    return cp == ' ' || cp == '\r';
}

TokenTypea getTokenType(string buffer, State state) {
    switch(state) {
        case State::ID:
            if (keywords.count(buffer))
                return TokenTypea::KEYWORD;
            return TokenTypea::ID;

        case State::INT:
            return TokenTypea::INT;

        case State::FLOAT:
            return TokenTypea::FLOAT;

        case State::OP:
            if (buffer == "*" || buffer == "/")
                return TokenTypea::MULTIPLICATIVE_OPERATOR;
            if (buffer == "<" || buffer == ">" || buffer == "==" || buffer == "<=" || buffer == ">=" || buffer == "!=")
                return TokenTypea::COMPARISON_OPERATOR;
            if (buffer == "=")
                return TokenTypea::ASSIGNMENT_OPERATOR;
            return TokenTypea::ADDITIVE_OPERATOR;

        case State::SEP:
            return TokenTypea::DELIMITER;

        default:
            return TokenTypea::ERR;
    }
}

list<Token> lexer(string text) {
    list<Token> tokens;
    State state = State::START;
    int start_column = 0, column = 0, line = 1;

    string::iterator it = text.begin();

    string buffer;
    while (it != text.end()) {
        auto charStart = it;
        CharType charType;
        uint32_t cp = utf8::next(it, text.end());

        column++;
        if (isLetter(cp)) charType = CharType::LETTER;
        else if (isDigit(cp)) charType = CharType::DIGIT;
        else if (cp == '.') charType = CharType::DOT;
        else if (isSpace(cp)) charType = CharType::SPACE;
        else if (cp == '=' || cp == '+' || cp == '-' || cp == '/' || cp == '*' || cp == '<' || cp == '>' || cp == '!')
            charType = CharType::OP;
        else if (cp == '[' || cp == ']' || cp == '(' || cp == ')' || cp == ',' || cp == ':')
            charType = CharType::SEP;
        else if (cp == '\n') {
            charType = CharType::SPACE;
            line++;
            column = 0;
        }
        else
            charType = CharType::UNKNOWN;


        State next_state = MAP.at(state).at(charType);
        cout << stateToString(state) << " got " << charTypeToString(charType) << " at " << column << " -> " << stateToString(next_state)  << endl;

        if (state == State::START && charType == CharType::SPACE) {
            state = State::START;
            continue;
        }

        if (next_state == State::END) {
            column--;
            tokens.push_back(Token(getTokenType(buffer, state), buffer, line, column));
            buffer.clear();
            state = State::START;
            it = charStart;
        } else {
            buffer.append(charStart, it);
            state = next_state;
        }
    }
    return tokens;
}


int main() {
    setlocale(LC_ALL, ".utf8");
    ifstream in("C:/Users/akzak/Desktop/TSU/compilation/test.zmii", ios::binary);
    string text((istreambuf_iterator<char>(in)), {});

    list<Token> tokens = lexer(text);
    for (const Token& token : tokens) {
        cout << token.repr() << endl;
    }

    in.close();

    cout << "ВПЕРЕД СЛАВЯНЕ";
    return 0;
}
