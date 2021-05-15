//
// Created by Zou on 4/18/21.
//

#ifndef MIDL_SCANNER_DFA_H
#define MIDL_SCANNER_DFA_H

enum class DFA {
    START = 0,
    STRING,
    ID,
    INTEGER,
    DOUBLE_SIGNAL
};

enum class STRING {
    STRING_0,
    STRING_1,
    STRING_2,
    STRING_4
};

enum class INTEGER {
    INTEGER_0,
    INTEGER_34,
    INTEGER_1234,
    INTEGER_4
};

enum class DOUBLE_SIGNAL {
    DOUBLE_SIGNAL_0,
    DOUBLE_SIGNAL_1,
    DOUBLE_SIGNAL_2,
    DOUBLE_SIGNAL_3
};

enum class ID {
    ID_0,
    ID_1234,
    ID_3,
    ID_234
};

#endif //MIDL_SCANNER_DFA_H
