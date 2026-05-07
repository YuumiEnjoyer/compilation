#include <iostream>
#include <fstream>
#include <map>
#include <cstdint>
#include <Windows.h>
#include <utf8.h>
#include <string>

using namespace std;

enum class State { START, ID, INT, FLOAT_DOT, FLOAT, OP, END, ERR, SEP };
enum class CharType { LETTER, DIGIT, DOT, OP, SPACE, UNKNOWN, SEP };

string stateToString(State state) {
    switch (state) {
        case State::START: return "START";
        case State::ID: return "ID";
        case State::INT: return "INT";
        case State::FLOAT_DOT: return "FLOAT_DOT";
        case State::FLOAT: return "FLOAT";
        case State::OP: return "OP";
        case State::END: return "END";
        case State::ERR: return "ERR";
        case State::SEP: return "SEP";
        default: return "ERR";
    }
}
string charTypeToString(CharType ct) {
    switch (ct) {
        case CharType::LETTER: return "LETTER";
        case CharType::DIGIT: return "DIGIT";
        case CharType::DOT: return "DOT";
        case CharType::SPACE: return "SPACE";
        case CharType::OP: return "OP";
        case CharType::UNKNOWN:return "UNKNOWN";
        case CharType::SEP: return "SEP";
        default: return "ERROR";
    }
}

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
    return cp == ' ' || cp == '\n' || cp == '\r' || cp == '\t';
}

map<State, map<CharType, State>> MAP = {
    {State::START, {
        {CharType::LETTER, State::ID},
        {CharType::DIGIT, State::INT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::OP},
        {CharType::SPACE, State::START},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::SEP},
    }},

    {State::ID, {
        {CharType::LETTER, State::ID},
        {CharType::DIGIT, State::ID},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::INT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::INT},
        {CharType::DOT, State::FLOAT_DOT},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::FLOAT_DOT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::FLOAT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::ERR},
        {CharType::SPACE, State::ERR},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::ERR},
    }},

    {State::FLOAT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::FLOAT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::OP, {
        {CharType::LETTER, State::END},
        {CharType::DIGIT, State::END},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::ERR, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::ERR},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::ERR},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::ERR},
    }},

    {State::SEP, {
        {CharType::LETTER, State::END},
        {CharType::DIGIT, State::END},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},
};


int main() {
    setlocale(LC_ALL, ".utf8");
    ifstream in("../../test.zmii", ios::binary);
    string text((istreambuf_iterator<char>(in)), {});

    State state = State::START;

    if (!utf8::is_valid(text.begin(), text.end())) {
        cerr << "Файл содержит некорректный UTF-8!" << endl;
        return 1;
    }

    string::iterator it = text.begin();

    string buffer;
    while (it != text.end()) {
        auto charStart = it;
        CharType charType;
        uint32_t cp = utf8::next(it, text.end());

        if (isLetter(cp)) charType = CharType::LETTER;
        else if (isDigit(cp)) charType = CharType::DIGIT;
        else if (cp == '.') charType = CharType::DOT;
        else if (isSpace(cp)) charType = CharType::SPACE;
        else if (cp == '=' || cp == '+' || cp == '-' || cp == '/' || cp == '*')
            charType = CharType::OP;
        else if (cp == '[' || cp == ']' || cp == '(' || cp == ')' || cp == ',')
            charType = CharType::SEP;
        else
            charType = CharType::UNKNOWN;


        State next_state = MAP[state][charType];
        cout << stateToString(state) << " got " << charTypeToString(charType) << " -> " << stateToString(next_state) << endl;
        State newState = State::ERR;

        if (state == State::START && charType == CharType::SPACE) {
            state = State::START;
            continue;
        }

        if (MAP[state].count(charType)) {
            newState = MAP[state][charType];
        }

        if (newState == State::END) {
            cout << "TOKEN: " << buffer << endl;
            buffer.clear();
            state = State::START;
            it = charStart;
        } else {
            buffer.append(charStart, it);
            state = newState;
        }
    }

    in.close();

    cout << "ВПЕРЕД СЛАВЯНЕ";
    return 0;
}
