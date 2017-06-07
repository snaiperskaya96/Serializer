//
// Created by Jury Verrigni on 06/06/2017.
//

#ifndef SERIALIZER_SERIALIZATION_H
#define SERIALIZER_SERIALIZATION_H

#include <SBuffer.h>
#include <Serializer.h>

#define SERIALIZE_DESERIALIZE(Type, Const) \
    Type OutValue; \
    Buffer << Const; \
    Buffer >> OutValue; \
    ASSERT_EQ(OutValue, Const);


namespace {
    class SerializationTest : public ::testing::Test {
    protected:
        SBuffer Buffer;
        const int IntegerValue = 10010;
        const float FloatValue = 10.451f;
        const double DoubleValue = 10.10;
        const size_t SizeValue = 60003;
        const std::string StringValue = "Hola!";

        SerializationTest() {
            // You can do set-up work for each test here.
        }

        virtual ~SerializationTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            //Buffer = SBuffer();
        }

        virtual void TearDown() {
            free(Buffer.Buffer);
            Buffer.SerializationIndex = 0;
            Buffer.BufferSize = 0;
            Buffer.DeserializationIndex = 0;
            Buffer.Buffer = malloc(SBUFFER_INITIAL_SIZE);
            Buffer.BufferSize = SBUFFER_INITIAL_SIZE;
        }
    };

    /**
     * Makes sure some basics type are correctly serialized and deserialized
     */
    TEST_F(SerializationTest, Integer) {
        SERIALIZE_DESERIALIZE(int, IntegerValue)
    }

    TEST_F(SerializationTest, Float) {
        SERIALIZE_DESERIALIZE(float, FloatValue)
    }

    TEST_F(SerializationTest, Double) {
        SERIALIZE_DESERIALIZE(double, DoubleValue)
    }

    TEST_F(SerializationTest, String) {
        SERIALIZE_DESERIALIZE(std::string, StringValue)
    }

    TEST_F(SerializationTest, Size) {
        SERIALIZE_DESERIALIZE(size_t, SizeValue);
    }

    TEST_F(SerializationTest, MultipleValuesRightOrder) {
        Buffer << IntegerValue;
        Buffer << FloatValue;
        Buffer << DoubleValue;
        Buffer << StringValue;
        Buffer << SizeValue;

        int NewInt;
        float NewFloat;
        double NewDouble;
        std::string NewString;
        size_t NewSize;

        Buffer >> NewInt;
        Buffer >> NewFloat;
        Buffer >> NewDouble;
        Buffer >> NewString;
        Buffer >> NewSize;

        ASSERT_TRUE(
                IntegerValue == NewInt &&
                FloatValue == NewFloat &&
                DoubleValue == NewDouble &&
                StringValue == NewString &&
                SizeValue == NewSize
        );
    }

    TEST_F(SerializationTest, MultipleValuesWrongOrder) {
        Buffer << IntegerValue;
        Buffer << FloatValue;
        Buffer << DoubleValue;
        Buffer << SizeValue;

        int NewInt;
        float NewFloat;
        double NewDouble;
        size_t NewSize;

        Buffer >> NewSize;
        Buffer >> NewFloat;
        Buffer >> NewInt;
        Buffer >> NewDouble;

        ASSERT_FALSE(
                IntegerValue == NewInt &&
                FloatValue == NewFloat &&
                DoubleValue == NewDouble &&
                SizeValue == NewSize
        );
    }


}
#endif //SERIALIZER_SERIALIZATION_H
