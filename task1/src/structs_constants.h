#pragma once

const unsigned int SIZE_SURNAME = 50;
const unsigned int SIZE_NAME = 50;
const unsigned int SIZE_PATRONYMIC = 50;
const unsigned int SIZE_CITY = 50;
const unsigned int SIZE_STREET = 50;
const unsigned int SIZE_BIRTH = 9;
const unsigned int MAX_COUNT_STRUCT = 400;

struct Address {
    char city[SIZE_CITY] = {};
    char street[SIZE_STREET] = {};
    unsigned int house = 0;
    unsigned int apartment = 0;
};

struct Base {
    char surname[SIZE_SURNAME] = {};
    char name[SIZE_NAME] = {};
    char patronymic[SIZE_PATRONYMIC] = {};
    
    char birth[SIZE_BIRTH] = {}; 

    char education[2] = {};      
    char marital_status[2] = {};
    int count_children = 0; // Целое число
    char profession[2] = {};     // Символ

    Address address;

    int phone;
};

struct City {
    char city[SIZE_CITY] = {};
    int code = 0;
};

struct Result_a {
    char surname[SIZE_SURNAME] = {};
    char name[SIZE_NAME] = {};
    char patronymic[SIZE_PATRONYMIC] = {};

    char birth[SIZE_BIRTH] = {};
    Address address;
    int phone = 0;
    int code = 0;
};

struct Result_b {
    char surname[SIZE_SURNAME] = {};
    char name[4] = {};
    char patronymic[4] = {};

    char birth[SIZE_BIRTH] = {};

    int phone = 0;
    int code = 0;
};