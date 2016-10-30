
#ifndef TEST_ENV_H_INCLUDED
#define TEST_ENV_H_INCLUDED

#define EXIT_OK          0
#define EXIT_ASSERT      1
#define END_OF_LINE      "\n\r"

#define assert_printer_3_ops(expected, lhs, rhs) \
    do {\
        const char * lhs_str = PRINTF_DEC_FORMAT(lhs);\
        const char * rhs_str = PRINTF_DEC_FORMAT(rhs);\
        fprintf(stderr, "Error at line %d of file %s." END_OF_LINE \
                        "Expecting " #lhs " " expected " " #rhs END_OF_LINE, __LINE__, __FILE__);\
        fprintf(stderr, "  Actual: ");\
        fprintf(stderr, lhs_str, lhs);\
        fprintf(stderr, END_OF_LINE);\
        fprintf(stderr, "Expected: ");\
        fprintf(stderr, rhs_str, rhs);\
        fprintf(stderr, END_OF_LINE);\
        fflush(stderr);\
        _Exit(EXIT_ASSERT);\
    } while (0)

#define assert_printer_2_ops(expected, lhs) \
    do {\
        const char * lhs_str = PRINTF_DEC_FORMAT(lhs);\
        fprintf(stderr, "Error at line %d of file %s." END_OF_LINE \
                        "Expecting " #lhs " " expected " " END_OF_LINE, __LINE__, __FILE__);\
        fprintf(stderr, "  Actual: ");\
        fprintf(stderr, lhs_str, lhs);\
        fprintf(stderr, END_OF_LINE);\
        fflush(stderr);\
        _Exit(EXIT_ASSERT);\
    } while (0)

#define ASSERT_EQ(lhs, rhs) \
    do { \
        if (!(lhs == rhs)) {\
            assert_printer_3_ops("to be equal to", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_NE(lhs, rhs) \
    do { \
        if (!(lhs != rhs)) {\
            assert_printer_3_ops("to be not equal to", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_GE(lhs, rhs) \
    do { \
        if (!(lhs >= rhs)) {\
            assert_printer_3_ops("to be greater or equal to", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_GT(lhs, rhs) \
    do { \
        if (!(lhs != rhs)) {\
            assert_printer_3_ops("to be greater than", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_LE(lhs, rhs) \
    do { \
        if (!(lhs != rhs)) {\
            assert_printer_3_ops("to be less or equal to", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_LT(lhs, rhs) \
    do { \
        if (!(lhs != rhs)) {\
            assert_printer_3_ops("to be less than", lhs, rhs);\
        }\
    } while (0)

#define ASSERT_TRUE(lhs) \
    do { \
        if (!(lhs)) {\
            assert_printer_2_ops("to be true", lhs);\
        }\
    } while (0)

#define ASSERT_FALSE(lhs) \
    do { \
        if (!(!lhs)) {\
            assert_printer_2_ops("to be false", lhs);\
        }\
    } while (0)

#define FAIL(lhs) \
    do { \
        fprintf(stderr, "Error at line %d of file %s." END_OF_LINE \
                        "Description: " #lhs END_OF_LINE, __LINE__, __FILE__);\
    } while (0)

#endif /* TEST_ENV_H_INCLUDED */
