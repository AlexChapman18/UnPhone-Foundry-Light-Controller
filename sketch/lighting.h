#include <stdint.h>


class Fixture {
    public:
        Fixture();
        Fixture(uint8_t _address);
        
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t address;

        void setRed(uint8_t _red);
        void setGreen(uint8_t _green);
        void setBlue(uint8_t _blue);
        void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue);

        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();
        uint8_t getAddress();

        uint8_t getRGB(uint8_t *_arr);
};


class FixtureGroup {
    public:
        FixtureGroup();
        FixtureGroup(Fixture* _fixtures, uint8_t _num_fixtures);
        
        uint8_t num_fixtures;
        Fixture* fixtures;

        void setGlobalRGB(uint8_t _red, uint8_t _green, uint8_t _blue);

        Fixture* getFixtures();
        uint8_t getNumFixtures();
        
        void print();

};